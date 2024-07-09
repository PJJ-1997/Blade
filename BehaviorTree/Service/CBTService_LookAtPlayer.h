// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_LookAtPlayer.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_LookAtPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_LookAtPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float DetectRadius;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
