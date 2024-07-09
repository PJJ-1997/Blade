// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_BossRangeAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Characters/Boss/CActor_RangeSword.h"
#include "Characters/CPlayer.h"

UCSubAction_BossRangeAttack::UCSubAction_BossRangeAttack()
{

}

void UCSubAction_BossRangeAttack::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnSubActionMode();

	CLog::Print("SubAction Boss Pressed");

	ActionData.DoAction(Owner);
}
// Auar   Spawn
void UCSubAction_BossRangeAttack::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();


	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector baseLocation = Owner->GetActorLocation();
	FRotator baseRotation = Owner->GetActorRotation();
	FVector offset = FVector(100.0f, 0.0f, 0.0f); // 각 액터 간의 오프셋을 설정

	int32 numActorsToSpawn = 5; // 스폰할 액터의 수를 지정

	for (int32 i = 0; i < numActorsToSpawn; ++i)
	{
		FTransform transform;
	
		FVector spawnLocation = baseLocation + baseRotation.RotateVector(SwordLocation) + offset * i;
		transform.SetLocation(spawnLocation);
		transform.SetRotation(FQuat(baseRotation));
	
		Owner->GetWorld()->SpawnActor<ACActor_RangeSword>(SwordClass, transform, params);
	}
}

void UCSubAction_BossRangeAttack::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	// State
	State->SetIdleMode();
	State->OffSubActionMode();

	// Movement
	Movement->Move();
	Movement->DisableFixedCamera();


}
