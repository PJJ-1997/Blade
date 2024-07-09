// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Distance.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_Distance : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Distance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
