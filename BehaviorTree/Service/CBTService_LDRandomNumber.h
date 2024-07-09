// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_LDRandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_LDRandomNumber : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_LDRandomNumber();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
