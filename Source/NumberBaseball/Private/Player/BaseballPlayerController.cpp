// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseballPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseballPlayerController::ABaseballPlayerController()
{
	bShowMouseCursor = true;
}

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}

void ABaseballPlayerController::ClientReceiveMessage_Implementation(const FString& Message, const FLinearColor& MessageColor)
{
	if (HasAuthority())
		UKismetSystemLibrary::PrintString(GetWorld(), Message, true, true, MessageColor, 10);
}

