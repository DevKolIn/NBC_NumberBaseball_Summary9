// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseballHUD.h"
#include "Blueprint/UserWidget.h"

void ABaseballHUD::BeginPlay()
{
	Super::BeginPlay();
	
	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}
