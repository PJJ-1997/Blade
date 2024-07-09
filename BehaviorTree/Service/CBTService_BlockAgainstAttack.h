// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_BlockAgainstAttack.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_BlockAgainstAttack : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_BlockAgainstAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
