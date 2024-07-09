// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CBossSpawner.h"
#include "Engine/World.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"

// 레벨 시작시 레벨 블루프린트에서 시네마틱 사용으로 바꿈
// 소환시 AIController 설정
ACBossSpawner::ACBossSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	BossScale = FVector(1.5f, 1.5f, 1.5f);
}

void ACBossSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACBossSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACBossSpawner::SpawnBoss()
{
	if (BossClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACBoss_AI* SpawnedBoss = GetWorld()->SpawnActor<ACBoss_AI>(BossClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedBoss)
		{
			SpawnedBoss->SetActorScale3D(BossScale);

			if (SpawnedBoss->GetController() == nullptr)
			{
				SpawnedBoss->SpawnDefaultController();
			}

			ACAIController_Boss* AIController = Cast<ACAIController_Boss>(SpawnedBoss->GetController());
			if (AIController == nullptr)
			{
				AIController = GetWorld()->SpawnActor<ACAIController_Boss>(ACAIController_Boss::StaticClass(), SpawnedBoss->GetActorLocation(), SpawnedBoss->GetActorRotation());
				if (AIController)
				{
					SpawnedBoss->Controller = AIController;
					AIController->Possess(SpawnedBoss);
					UE_LOG(LogTemp, Warning, TEXT("Explicitly set AI Controller for spawned boss"));
				}
			}
			if (AIController)
			{
				AIController->StartAI();
				UE_LOG(LogTemp, Warning, TEXT("Spawned Boss AI started"));
			}
		}
	}
	//if (BossClass)
	//{
	//	FActorSpawnParameters SpawnParams;
	//	ACBoss_AI* SpawnedBoss = GetWorld()->SpawnActor<ACBoss_AI>(BossClass, SpawnLocation, SpawnRotation, SpawnParams);
	//
	//	if (SpawnedBoss)
	//	{
	//		SpawnedBoss->SetActorScale3D(BossScale);
	//
	//		if (SpawnedBoss->GetController() == nullptr)
	//		{
	//			SpawnedBoss->SpawnDefaultController();
	//		}
	//	}
	//}
}
