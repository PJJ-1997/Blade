// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CItemStructsDataComponent.generated.h"

UENUM()
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Potion UMETA(DisplayName = "Potion")
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FText Name;
	UPROPERTY(EditAnywhere)
		FText Description;
	UPROPERTY(EditAnywhere)
		FText InteractionText;
	UPROPERTY(EditAnywhere)
		FText UsageText;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float Weight;
	UPROPERTY(EditAnywhere)
		int32 MaxStackSize;
	UPROPERTY(EditAnywhere)
		bool bIsStackable;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FName ID;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemAssetData AssetData;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U06_BATTLE_API UCItemStructsDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCItemStructsDataComponent();


};
