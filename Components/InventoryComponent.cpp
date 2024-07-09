// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "ItemData/ItemBase.h"
#include "Components/CStatusComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

UItemBase * UInventoryComponent::FindMatchingItem(UItemBase * ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase * UInventoryComponent::FindNextItemByID(UItemBase * ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<UItemBase*>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase * UInventoryComponent::FindNextPartialStack(UItemBase * ItemIn) const
{
	if (const TArray<UItemBase*>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
			{
				return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			}
		))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase * ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase * StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase * ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase * ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);
	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase * ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase *InputItem)
{
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the Inventory. Item has invalid weight value."), InputItem->TextData.Name));
	}

	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the Inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the Inventory. All inventory slots are full."), InputItem->TextData.Name));
	}
	AddNewItem(InputItem,1);
	return FItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added a single {0} to the inventory."), InputItem->TextData.Name));

	return FItemAddResult();
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		//invalid item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

		// check if the input already exists in the inventory and is not a full stack
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	while (ExistingItemStack)
	{
		// calculate how many of the existing item would be needed to make the next full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		if (WeightLimitAddAmount > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			//TODO: Refine this logic since going over weight capacity should not ever be possible
			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase * InputItem)
{

	if (GetOwner())
	{
		const int32 InitalRequestedAddAmount = InputItem->Quantity;
		// handle non stackable items
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitalRequestedAddAmount);

		if (StackableAmountAdded == InitalRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitalRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."), InitalRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitalRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No reamining inventory slots, or invalid item."), InputItem->TextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwenr() check some how failed."));
}

void UInventoryComponent::AddNewItem(UItemBase * Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::HasItemWithID(const FName & ItemID) const
{
	for (UItemBase* Item : InventoryContents)
	{
		if (Item && Item->ID == ItemID)
		{
			return true;
		}
	}
	return false;
}
