// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Cooldown.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCUserWidget_Cooldown : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Cooldown")
		void UpdateCooldown(FName SkillName, float CooldownTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
		void OnCooldownUpdate(FName SkillName, float RemainingTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Cooldown")
		void ShowCooldown(FName SkillName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
		void HideCooldown(FName SkillName);
};
