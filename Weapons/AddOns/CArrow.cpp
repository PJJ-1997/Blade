// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/AddOns/CArrow.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Characters/CBoss.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACArrow::ACArrow()
{
	PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Static, "Static", Capsule);
    CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "NiagaraComponent", Static);

    Projectile->ProjectileGravityScale = 1.0f;
    Capsule->BodyInstance.bNotifyRigidBodyCollision = true;  // C++ true에서는 True로
    Capsule->SetCollisionProfileName("BlockAll");


}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

    Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);
    Projectile->Deactivate();
	
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // 화살이 삭제되기전에 배열에 저장한것에서 삭제하기 위해
    if (OnEndPlay.IsBound())
        OnEndPlay.Broadcast(this);

}

void ACArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 소켓 변환 얻기
	FTransform SocketTransform = Static->GetSocketTransform(TEXT("Socket"), RTS_World);
	FVector SocketLocation = SocketTransform.GetLocation();
	FRotator SocketRotation = SocketTransform.GetRotation().Rotator();

	// 전방 벡터 얻기 및 상수 곱하기 (예: 1000.0f)
	FVector LaunchVelocity = SocketRotation.Vector() * 2000.0f;

	// 객체 유형 배열 정의
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	// PredictProjectilePath 함수 호출
	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = SocketLocation;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.ProjectileRadius = 1.0f;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ObjectTypes = ObjectTypes;
	PredictParams.MaxSimTime = 2.0f;
	PredictParams.OverrideGravityZ = 0.0f;
	PredictParams.bTraceComplex = false;

	FPredictProjectilePathResult PredictResult;
	bool bPathValid = UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);


	// 디버그 경로 그리기
	if (bPathValid)
	{
		for (const FPredictProjectilePathPointData& Point : PredictResult.PathData)
		{
			//DrawDebugSphere(GetWorld(), Point.Location, 5.0f, 12, FColor::Red, false, 0.1f);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PathEffect, Point.Location, FRotator::ZeroRotator);
		}
	}
}

void ACArrow::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
    SetLifeSpan(LifeSpanAfterCollision);

    for (AActor* actor : Ignores)
        CheckTrue(actor == OtherActor)

     Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ACharacter* character = Cast<ACharacter>(OtherActor);
    if (!!character && OnHit.IsBound())
        OnHit.Broadcast(this, character);

	if (character)
	{
		if (ArrowType == EArrowType::Fire)
		{
			ACEnemy* Enemy = Cast<ACEnemy>(character);
			if (Enemy)
			{
				Enemy->ApplyFireDamage(5.0f, 3.0f);
				Enemy->PlayHitEffect(FireHitEffect, 3.0f);
			}
		}
		else if (ArrowType == EArrowType::Ice)
		{
			ACEnemy* Enemy = Cast<ACEnemy>(character);
			if (Enemy)
			{
				Enemy->ApplyIceEffect(3.0f);
				Enemy->PlayHitEffect(IceHitEffect, 3.0f);
			}
		}
	}

	ACBoss* Boss = Cast<ACBoss>(character);
	if (Boss)
	{
		if (ArrowType == EArrowType::Fire)
		{
			Boss->ApplyFireDamage(5.0f, 3.0f);
			Boss->PlayHitEffect(FireHitEffect, 3.0f);
		}
		else if (ArrowType == EArrowType::Ice)
		{
			Boss->ApplyIceEffect(3.0f);
			Boss->PlayHitEffect(IceHitEffect, 3.0f);
		}
	}
}

void ACArrow::Shoot(const FVector & InFoward)
{
    Projectile->Velocity = InFoward * Projectile->InitialSpeed;
    Projectile->Activate();

    Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bHasBeenShot = true;  // 화살이 발사되었음을 표시
}

void ACArrow::SetNiagaraEffect(UNiagaraSystem * NiagaraSystem)
{
	if (NiagaraComponent && NiagaraSystem)
	{
		NiagaraComponent->SetAsset(NiagaraSystem);
		NiagaraComponent->Activate(true);
	}
}

void ACArrow::SetArrowType(EArrowType NewType)
{
	ArrowType = NewType;
}
