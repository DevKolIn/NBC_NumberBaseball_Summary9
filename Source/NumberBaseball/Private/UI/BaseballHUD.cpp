// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseballHUD.h"
#include "Blueprint/UserWidget.h"

ABaseballHUD::ABaseballHUD()
{
	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
}
