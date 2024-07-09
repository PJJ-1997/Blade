// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_TeleportToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTTaskNode_TeleportToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_TeleportToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Teleport")
		float DistanceInFrontOfPlayer = 300.0f;
};
