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

	// �ݸ��� �ڽ� ���� �� ����
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Decal);
	//CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f)); // ũ�� ����
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAll")); // �ݸ��� ������ ����
}

void ACFloorPattern::BeginPlay()
{
	Super::BeginPlay();

	// �ݸ��� �ڽ��� ���� �� ��Ȱ��ȭ
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetHiddenInGame(true);

	// ��Į�� ��Ÿ���� Ÿ�̸�
	FTimerHandle DecalTimerHandle;
	GetWorldTimerManager().SetTimer(
		DecalTimerHandle,
		this,
		&ACFloorPattern::OnTimerFinished,
		0.5f, // ��Į�� ��Ÿ���µ� �ɸ��� �ð�
		false
	);

	// 1.2�� �Ŀ� �ݸ��� �ڽ� Ȱ��ȭ
	FTimerHandle BoxTimerHandle;
	FTimerDelegate EnableBoxDelegate = FTimerDelegate::CreateUObject(this, &ACFloorPattern::EnableCollisionBox);
	GetWorldTimerManager().SetTimer(
		BoxTimerHandle,
		EnableBoxDelegate,
		1.2f, // �ݸ��� �ڽ��� Ȱ��ȭ�ϴµ� �ɸ��� �ð�
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
		// �÷��̾�� ������ ����
		if (HitData.Montage)
		{
			// ��Ʈ ���Ÿ�� ���
			Player->PlayAnimMontage(HitData.Montage, HitData.PlayRate);
		}

		if (HitData.Sound)
		{
			// ��Ʈ ���� ���
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitData.Sound, GetActorLocation());
		}

		// �÷��̾�� ������ ����
		Player->TakeDamage(HitData.Power, FDamageEvent(), Player->GetController(), this);

		// �ݸ��� �ڽ� ��Ȱ��ȭ
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACFloorPattern::RemoveCollisionBox()
{
	// �ݸ��� �ڽ� ����
	if (CollisionBox)
	{
		CollisionBox->DestroyComponent();
		CollisionBox = nullptr;
	}
}

void ACFloorPattern::EnableCollisionBox()
{
	// �ݸ��� �ڽ� Ȱ��ȭ
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionBox->SetHiddenInGame(false);
	}
}