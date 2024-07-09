// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/ItemBase.h"
#include "Components/InventoryComponent.h"
#include "Components/CStatusComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;	
	bIsPickup = false;
}

UItemBase * UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != this->Quantity)
	{
		this->Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		if(OwningInventory)
		{
			if(this->Quantity <= 0)
			{
				this->OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemBase OwningInventory was null (item may be a pickup)."));
		}
	}
}

void UItemBase::Use(ACharacter * Character)
{
	ACPlayer* Player = Cast<ACPlayer>(Character);
	if (Player && ID == "test_002")
	{
		Player->Status->Heal(30);
		SetQuantity(Quantity - 1);
	}
}
