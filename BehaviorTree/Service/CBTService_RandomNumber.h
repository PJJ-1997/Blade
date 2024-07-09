// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_RandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_RandomNumber : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_RandomNumber();
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
