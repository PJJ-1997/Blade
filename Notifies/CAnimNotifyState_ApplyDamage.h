// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_ApplyDamage.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimNotifyState_ApplyDamage : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
private:
	// 데미지 관련 변수들
	float DamageAmount;
	FDamageEvent DamageEvent;
	AController* EventInstigator;
	AActor* DamageCauser;
};
