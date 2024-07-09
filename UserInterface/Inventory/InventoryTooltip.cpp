// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "ItemData/ItemBase.h"
#include "Components/TextBlock.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	UItemBase* ItemBeingHovered = InventorySlotBeginHovered->GetItemReference();
	switch(ItemBeingHovered->ItemType)
	{
	case EItemType::Potion:
		break;
	case EItemType::Weapon: // #8 58:28
		break;
	default:;
	}
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

	const FString WeightInfo = { "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
