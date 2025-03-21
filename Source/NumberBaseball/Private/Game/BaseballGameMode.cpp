// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseballGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/BaseballPlayerController.h"

ABaseballGameMode::ABaseballGameMode()
{
	bIsGameStart = false;
	bIsGameOver = false;
	MaxChance = 3;
}

void ABaseballGameMode::LoginUser(FString PlayerID)
{
	FPlayerInfo PlayerInfo;
	PlayerInfo.PlayerID = PlayerID;
	PlayerInfo.PlayerChance = MaxChance;
	PlayerInfo.MessageColor = GenerateBrightRandomColor();
	
	PlayerInfos.Emplace(PlayerID, PlayerInfo);
	
	if (CanGameStart())
		GameStart();
}

void ABaseballGameMode::GameStart()
{
	bIsGameStart = true;
	GenerateRandomNumbers();
	BroadcastPrintMessage(TEXT("[새 게임 시작] 숫자를 맞춰보세요!"));
}

void ABaseballGameMode::ReceiveMessageFromClient(FString PlayerID, FString Message)
{
	if (Message.Len() == 0)
		return;
	
	if (Message[0] == TEXT('/'))
	{
		HandleCommand(PlayerID, Message);
	}
	else
	{
		MulticastPrintMessage(PlayerID, Message);
	}
}

void ABaseballGameMode::GameReset()
{
	// 1. 게임 상태 초기화
	bIsGameStart = false;
	bIsGameOver = false;

	// 2. 플레이어들의 기회 초기화
	for (TPair<FString, FPlayerInfo>& Elem : PlayerInfos)
	{
		FPlayerInfo& PlayerInfo = Elem.Value;
		PlayerInfo.PlayerChance = MaxChance;
	}
	
	GameStart();
}

void ABaseballGameMode::GameEnd()
{
	// 1. 플레이어들에게 게임 종료 공지
	FString ReplyMessage = FString::Printf(TEXT("[게임 종료] 정답은 %s 였습니다."), *CurrentRandomNumber);
	BroadcastPrintMessage(ReplyMessage);

	bIsGameOver = true;
}

void ABaseballGameMode::GameRestart()
{
	// 1. 게임 종료
	GameEnd();
	
	// 2. 약간의 딜레이 후 게임 시작
	FTimerHandle RestartTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &ABaseballGameMode::GameReset, 3.0f, false);
}

void ABaseballGameMode::GenerateRandomNumbers()
{
	int32 RandomNumber = 0;
	
	TArray<int32> AvailableNumbers;

	for (int32 i = 1; i <= 9; ++i)
	{
		AvailableNumbers.Add(i);
	}
	
	for (int32 i = 0; i < 3; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, AvailableNumbers.Num() - 1);
		RandomNumber *= 10;
		RandomNumber += AvailableNumbers[RandomIndex];
		AvailableNumbers.RemoveAt(RandomIndex);
	}

	CurrentRandomNumber = FString::FromInt(RandomNumber);
}

FLinearColor ABaseballGameMode::GenerateBrightRandomColor()
{
	// H: 0 ~ 360 (색상), S: 0.7~1.0 (채도), V: 0.8~1.0 (밝기)
	float Hue = FMath::FRandRange(0.f, 360.f);
	float Saturation = FMath::FRandRange(0.7f, 1.0f);
	float Value = FMath::FRandRange(0.8f, 1.0f);

	FLinearColor HSVColor = FLinearColor::MakeFromHSV8(
		static_cast<uint8>(Hue / 360.f * 255.f),
		static_cast<uint8>(Saturation * 255.f),
		static_cast<uint8>(Value * 255.f)
	);

	return HSVColor;
}

void ABaseballGameMode::MulticastPrintMessage(const FString& PlayerID, const FString& Message) const
{
	MulticastPrintMessage(PlayerID + " : " + Message, PlayerInfos[PlayerID].MessageColor);
}

void ABaseballGameMode::MulticastPrintMessage(const FString& Message, const FLinearColor& MessageColor = FColor::Yellow) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseballPlayerController::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ABaseballPlayerController* PlayerController = Cast<ABaseballPlayerController>(Actor))
		{
			PlayerController->ClientReceiveMessage(Message, MessageColor);
		}
	}
}

void ABaseballGameMode::BroadcastPrintMessage(const FString& Message) const
{
	UKismetSystemLibrary::PrintString(GetWorld(), Message, true, true, FColor::Yellow, 10);
}

void ABaseballGameMode::HandleCommand(FString& PlayerID, FString& Message)
{
	// 01. 게임이 종료되었는가?
	if (bIsGameOver)
		return;
	
	// 02. 플레이어 아이디가 유효한가?
	FPlayerInfo* PlayerInfo = PlayerInfos.Find(PlayerID);
	if (PlayerInfo == nullptr)
		return;

	// 03. 플레이어에게 기회가 있는가?
	if (PlayerInfo->PlayerChance < 1)
	{
		FString ReplyMessage = FString::Printf(TEXT("[%s] 더 이상 도전할 수 없습니다."), *PlayerID);
		BroadcastPrintMessage(ReplyMessage);
		return;
	}
	PlayerInfo->PlayerChance -= 1;

	// 04. 플레이어 메시지를 클라이언트에게 전송
	MulticastPrintMessage(PlayerID, Message);

	// 05. 게임 결과 요청
	EAnswerResult Result = GetBaseballResult(*PlayerInfo, Message);
	
	// 06. 플레이어의 게임 결과 확인
	if (Result == EAnswerResult::Miss)
	{
		{
			FString ReplyMessage = FString::Printf(TEXT("[%s] %s -> %s"), *PlayerID, *PlayerInfo->Answer, *PlayerInfo->AnswerResult);
			BroadcastPrintMessage(ReplyMessage);
		}
		
		if (PlayerInfo->PlayerChance == 0)
		{
			FString ReplyMessage = FString::Printf(TEXT("%s가 기회를 모두 소진하였습니다. 다른 플레이어 승리!"), *PlayerID);
			BroadcastPrintMessage(ReplyMessage);
			GameEnd();
		}
		if (ShouldGameOver())
		{
			GameRestart();
		}
	}

	if (Result == EAnswerResult::Correct)
	{
		FString ReplyMessage = FString::Printf(TEXT("[%s] %s -> 3 Strikes OUT! Victory!"), *PlayerID, *PlayerInfo->AnswerResult);
		BroadcastPrintMessage(ReplyMessage);
		GameEnd();
	}
}

EAnswerResult ABaseballGameMode::GetBaseballResult(FPlayerInfo& PlayerInfo, FString& Message)
{
	int32 Strike = 0;
	int32 Ball = 0;

	FString Answer = Message.Mid(1);
	PlayerInfo.Answer = Answer;
	int32 LoopCount = FMath::Clamp(Answer.Len() ,0, 3);
	for (int32 i = 0; i < LoopCount; ++i)
	{
		if (Answer[i] == CurrentRandomNumber[i])
		{
			Strike++;
		}
		else if (CurrentRandomNumber.Contains(FString::Chr(Answer[i])))
		{
			Ball++;
		}
	}
	
	if (Strike < 3)
	{
		PlayerInfo.AnswerResult = FString::Printf(TEXT("%dB %dS"), Ball, Strike);
		return EAnswerResult::Miss;
	}
	
	return EAnswerResult::Correct;
}

bool ABaseballGameMode::ShouldGameOver()
{
	// 모든 플레이어들의 기회가 없을 경우 게임을 종료한다.
	for (const TPair<FString, FPlayerInfo>& Elem : PlayerInfos)
	{
		FPlayerInfo PlayerInfo = Elem.Value;
		if (PlayerInfo.PlayerChance >= 1)
			return false;
	}

	return true;
}

bool ABaseballGameMode::CanGameStart() const
{
	return bIsGameStart == false && PlayerInfos.Num() >= 2 && bIsGameOver == false;
}
