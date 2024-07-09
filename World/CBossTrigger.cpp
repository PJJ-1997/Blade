// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CBossTrigger.h"
#include "Components/BoxComponent.h"
#include "Characters/Boss/CBoss_AI.h"
#include "World/CBossSpawner.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// 레벨 시작시 레벨 블루프린트에서 시네마틱 사용으로 바꿈
ACBossTrigger::ACBossTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACBossTrigger::OnOverlapBegin);

	bHasTriggered = false;
}

void ACBossTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (BossSpawnerClass)
	{
		BossSpawner = GetWorld()->SpawnActor<ACBossSpawner>(BossSpawnerClass);
	}
}

void ACBossTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBossTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bHasTriggered && OtherActor && OtherActor != this && OtherComp && BossSpawner)
	{
		bHasTriggered = true;
		BossSpawner->SpawnBoss();

		TriggerBox->DestroyComponent();
	}

}

