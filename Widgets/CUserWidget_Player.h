// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Player.generated.h"

UCLASS()
class U06_BATTLE_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()

public:
	// Health 정보를 업데이트하는 함수
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float CurrentHealth, float MaxHealth);

	// Stamina 정보를 업데이트하는 함수
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent)
		void HighlightSwordImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
		void HighlightHammerImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
		void HighlightBowImage(bool bHighlight);
};
