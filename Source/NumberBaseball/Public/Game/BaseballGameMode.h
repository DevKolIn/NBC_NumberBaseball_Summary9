// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseballGameMode.generated.h"

enum EAnswerResult
{
	None,
	NoChance,
	Miss,
	Correct,
};

struct FPlayerInfo
{
	FString PlayerID;
	int32 PlayerChance;
	FLinearColor MessageColor;
	FString Answer;
	FString AnswerResult;
};

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API ABaseballGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseballGameMode();

	UFUNCTION(BlueprintCallable)
	void LoginUser(FString PlayerID);
	
	UFUNCTION(BlueprintCallable)
	void GameStart();

	UFUNCTION(BlueprintCallable)
	void ReceiveMessageFromClient(FString PlayerID, FString Message);
	
private:
	TMap<FString, FPlayerInfo> PlayerInfos;
	FString CurrentRandomNumber;
	bool bIsGameStart;
	bool bIsGameOver;
	int32 MaxChance;
	
	void GameReset();
	void GameEnd();
	void GameRestart();
	void GenerateRandomNumbers();
	FLinearColor GenerateBrightRandomColor();
	void MulticastPrintMessage(const FString& Message, const FLinearColor& MessageColor) const;
	void MulticastPrintMessage(const FString& PlayerID, const FString& Message) const;
	void BroadcastPrintMessage(const FString& PlayerID,const FString& Message) const;
	void BroadcastPrintMessage(const FString& Message) const;
	
	void HandleCommand(FString& PlayerID, FString& Message);
	EAnswerResult GetBaseballResult(FPlayerInfo& PlayerID, FString& Message);
	
	bool ShouldGameOver();
	bool CanGameStart() const;
};