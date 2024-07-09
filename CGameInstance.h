// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemData/ItemBase.h"
#include "CGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicFinishedDelegate);

UCLASS()
class U06_BATTLE_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCGameInstance();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Data")
		float PlayerHealth;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Data")
		float PlayerStamina;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Data")
		TArray<UItemBase*> PlayerInventory;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Data")
		bool bIsPlayerDataInitialized;

	void InitializePlayerData(float DefaultHealth, float DefaultStamina, const TArray<UItemBase*>& DefaultInventory);

	void SavePlayerData(float Health, float Stamina, TArray<UItemBase*> Inventory);
	void LoadPlayerData(float& Health, float& Stamina, TArray<UItemBase*>& Inventory);

};
