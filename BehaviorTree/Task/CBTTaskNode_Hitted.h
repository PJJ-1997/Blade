// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Hitted.generated.h"

UCLASS()
class U06_BATTLE_API UCBTTaskNode_Hitted : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Hitted();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
