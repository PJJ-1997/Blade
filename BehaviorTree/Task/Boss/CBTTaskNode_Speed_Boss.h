// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CMovementComponent.h"
#include "CBTTaskNode_Speed_Boss.generated.h"

UCLASS()
class U06_BATTLE_API UCBTTaskNode_Speed_Boss : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Speed_Boss();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	UPROPERTY(EditAnywhere, Category = "Type")
		ESpeedType Type;
};
