// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_BossRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCSubAction_BossRangeAttack : public UCSubAction
{
	GENERATED_BODY()

public:
	UCSubAction_BossRangeAttack();
private:
	UPROPERTY(EditAnywhere, Category = "SpawnClass")
		TSubclassOf<class ACActor_RangeSword> SwordClass;
	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
		FVector SwordLocation;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

public:
	virtual void Pressed() override;

public:
	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;


};
