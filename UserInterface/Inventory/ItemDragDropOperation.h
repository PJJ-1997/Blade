// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UItemBase;
class UInventoryComponent;

UCLASS()
class U06_BATTLE_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UItemBase* SourceItem;
	UPROPERTY()
		UInventoryComponent* SourceInventory;
};
