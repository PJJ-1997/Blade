// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_FKey.generated.h"

UCLASS()
class U06_BATTLE_API UCUserWidget_FKey : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "FKey")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "FKey")
		void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "FKey")
		void UpdateControllerName(const FString& InName);
};
