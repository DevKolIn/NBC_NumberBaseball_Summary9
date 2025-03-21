// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABaseballPlayerController();

	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void ClientReceiveMessage(const FString& Message, const FLinearColor& MessageColor);
};
