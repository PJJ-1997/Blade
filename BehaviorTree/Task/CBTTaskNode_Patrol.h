// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class U06_BATTLE_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Patrol();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode = true;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AcceptanceDistance = 20;
private:
	UPROPERTY(EditAnywhere, Category = "Random")
		float RandomRadius = 150;

private:
	void  DrawDebug(UWorld* InWorld, FVector InLocation);

};
