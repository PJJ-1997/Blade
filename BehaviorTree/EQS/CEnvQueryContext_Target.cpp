// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EQS/CEnvQueryContext_Target.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Enemy/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// BB에 있는 target을 가져오기 위해
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController*  controlller = ai->GetController<ACAIController>();

	UBlackboardComponent* blackboard = controlller->GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));   // Player


	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

};
