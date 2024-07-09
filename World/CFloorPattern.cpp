// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CFloorPattern.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

ACFloorPattern::ACFloorPattern()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(RootComponent);
	
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(Decal);
	Niagara->SetAutoActivate(false);

	// 콜리전 박스 생성 및 설정
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Decal);
	//CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f)); // 크기 설정
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAll")); // 콜리전 프로필 설정
}

void ACFloorPattern::BeginPlay()
{
	Super::BeginPlay();

	// 콜리전 박스를 시작 시 비활성화
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetHiddenInGame(true);

	// 데칼이 나타나는 타이머
	FTimerHandle DecalTimerHandle;
	GetWorldTimerManager().SetTimer(
		DecalTimerHandle,
		this,
		&ACFloorPattern::OnTimerFinished,
		0.5f, // 데칼이 나타나는데 걸리는 시간
		false
	);

	// 1.2초 후에 콜리전 박스 활성화
	FTimerHandle BoxTimerHandle;
	FTimerDelegate EnableBoxDelegate = FTimerDelegate::CreateUObject(this, &ACFloorPattern::EnableCollisionBox);
	GetWorldTimerManager().SetTimer(
		BoxTimerHandle,
		EnableBoxDelegate,
		1.2f, // 콜리전 박스를 활성화하는데 걸리는 시간
		false
	);
}

void ACFloorPattern::OnTimerFinished()
{
	// Hide the decal
	Decal->SetVisibility(false);

	// Activate the Niagara system
 	Niagara->Activate();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACFloorPattern::RemoveCollisionBox, 1.0f, false);
}

void ACFloorPattern::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	if (Player)
	{
		// 플레이어에게 데미지 적용
		if (HitData.Montage)
		{
			// 히트 모션타주 재생
			Player->PlayAnimMontage(HitData.Montage, HitData.PlayRate);
		}

		if (HitData.Sound)
		{
			// 히트 사운드 재생
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitData.Sound, GetActorLocation());
		}

		// 플레이어에게 데미지 적용
		Player->TakeDamage(HitData.Power, FDamageEvent(), Player->GetController(), this);

		// 콜리전 박스 비활성화
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACFloorPattern::RemoveCollisionBox()
{
	// 콜리전 박스 제거
	if (CollisionBox)
	{
		CollisionBox->DestroyComponent();
		CollisionBox = nullptr;
	}
}

void ACFloorPattern::EnableCollisionBox()
{
	// 콜리전 박스 활성화
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionBox->SetHiddenInGame(false);
	}
}