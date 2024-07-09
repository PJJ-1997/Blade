// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Label.generated.h"

UCLASS()
class U06_BATTLE_API UCUserWidget_Label : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateControllerName(const FString& InName);
};
