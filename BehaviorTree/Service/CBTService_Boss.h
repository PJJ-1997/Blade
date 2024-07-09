// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss.generated.h"

class ACBoss_AI;
class UCAIBehaviorComponent;

UCLASS()
class U06_BATTLE_API UCBTService_Boss : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Boss();

protected:
	// BP¿¡¼­ Recive Tick AI
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float  ActionRange = 150.0f;
};
