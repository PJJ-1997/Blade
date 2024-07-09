#include "Weapons/CWeaponstructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Sound/SoundWave.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimMontage.h"

void FDoActionData::DoAction(ACharacter * InOwner)
{
    UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
    if (!!movement)
    {
        if (this->bFixedCamera)
            movement->EnableFixedCamera();
        if (this->bCanMove == false)
            movement->Stop();

    }
    if (!!this->Montage)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::SendDamage(ACharacter * InAttacker, AActor * InAttackCauser, ACharacter * InOther)
{
   FActionDamageEvent e;
   e.HitData = this;
   
   InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter * InOwner)
{
    CheckNull(Montage);

    if(!!InOwner)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld * InWorld)
{
    // stop time이 0.0f인경우는 return;
    StopPawns.Empty();

    CheckTrue(FMath::IsNearlyZero(StopTime));

    // 현재 Level에서 모두 찾는다
    for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
    {
        APawn* pawn = Cast<ACharacter>(actor);
        if (!!pawn)
        {
            pawn->CustomTimeDilation = 1e-3f; // 0.001f
            StopPawns.Add(pawn);
        }
    }

    FTimerDelegate timerDeleagte;

    // BindUFunction대신에 BindLambda를 이용
    // 별도의 클래스내에 만들지 않고 유효영역내에서만 사용하곘다

    timerDeleagte.BindLambda([=]()
        {
            for (APawn* pawn : StopPawns)
            {
                pawn->CustomTimeDilation = 1;

            }

        });

    FTimerHandle  handle;
    InWorld->GetTimerManager().SetTimer(handle, timerDeleagte, StopTime, false);
}
void FHitData::PlaySoundWave(class ACharacter* InOwner)
{
    CheckNull(Sound);
    CheckNull(InOwner);

    UWorld* world = InOwner->GetWorld();
    FVector location = InOwner->GetActorLocation();
    UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);


}


void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
	CheckNull(Effect);
	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));  // FVector,FRotator
	transform.SetScale3D(EffectScale); // 스케일 설정

	CHelpers::PlayEffect(InWorld, Effect, transform);
   //CheckNull(Effect);
   //FTransform transform;
   //transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));  // FVector,FRotator
   //
   //CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FHitData::PlayBossMontage(ACharacter * InOwner)
{
	CheckNull(BMontage);

	if (!!InOwner)
		InOwner->PlayAnimMontage(BMontage, BPlayRate);
}

void FHitData::PlayBossHitStop(UWorld * InWorld)
{ // stop time이 0.0f인경우는 return;
	StopPawns.Empty();

	CheckTrue(FMath::IsNearlyZero(StopTime));

	// 현재 Level에서 모두 찾는다
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);
		if (!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f; // 0.001f
			StopPawns.Add(pawn);
		}
	}

	FTimerDelegate timerDeleagte;

	// BindUFunction대신에 BindLambda를 이용
	// 별도의 클래스내에 만들지 않고 유효영역내에서만 사용하곘다

	timerDeleagte.BindLambda([=]()
		{
			for (APawn* pawn : StopPawns)
			{
				pawn->CustomTimeDilation = 1;

			}

		});

	FTimerHandle  handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDeleagte, StopTime, false);
}

void FHitData::PlayBossSoundWave(ACharacter * InOwner)
{
	CheckNull(BSound);
	CheckNull(InOwner);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();
	UGameplayStatics::SpawnSoundAtLocation(world, BSound, location);
}

void FHitData::PlayBossEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
	CheckNull(BEffect);
	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));  // FVector,FRotator

	CHelpers::PlayEffect(InWorld, BEffect, transform);
}

