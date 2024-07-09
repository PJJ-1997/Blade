// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class U06_BATTLE_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Melee();

protected:
	// BP¿¡¼­ Recive Tick AI
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float  ActionRange = 150.0f;
};
