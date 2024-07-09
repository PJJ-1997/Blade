// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameInstance.h"
UCGameInstance::UCGameInstance()
	: PlayerHealth(100.0f) // Default value
	, PlayerStamina(100.0f) // Default value
	, bIsPlayerDataInitialized(false)
{

}
void UCGameInstance::InitializePlayerData(float DefaultHealth, float DefaultStamina, const TArray<UItemBase*>& DefaultInventory)
{
	PlayerHealth = DefaultHealth;
	PlayerStamina = DefaultStamina;
	PlayerInventory = DefaultInventory;
	bIsPlayerDataInitialized = true;

}

void UCGameInstance::SavePlayerData(float Health, float Stamina, TArray<UItemBase*> Inventory)
{
	PlayerHealth = Health;
	PlayerStamina = Stamina;
	PlayerInventory = Inventory;

}

void UCGameInstance::LoadPlayerData(float & Health, float & Stamina, TArray<UItemBase*>& Inventory)
{
	Health = PlayerHealth;
	Stamina = PlayerStamina;
	Inventory = PlayerInventory;

}
