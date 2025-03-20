// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseballHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API ABaseballHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABaseballHUD();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUserWidget> HUDWidget;
};
