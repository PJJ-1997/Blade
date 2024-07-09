// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_LowHealth.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTDecorator_LowHealth : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCBTDecorator_LowHealth();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
