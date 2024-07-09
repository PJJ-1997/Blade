// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_SpawnDecal.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "World/CFloorPattern.h"

UCBTService_SpawnDecal::UCBTService_SpawnDecal()
{
	bCreateNodeInstance = true;
	this->NodeName = "SpawnDecal";
}

void UCBTService_SpawnDecal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	SpawnActors(OwnerComp);
}

void UCBTService_SpawnDecal::SpawnActors(UBehaviorTreeComponent& OwnerComp)
{
	if (!ActorToSpawn) return;
	
	ACAIController_Boss* AIController = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner());
	if (!AIController) return;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;
	
	ACPlayer* Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player) return;
	
	FVector PlayerLocation = Player->GetActorLocation();
	UWorld* World = GetWorld();
	
	TArray<FVector> SpawnLocations;
	const float MinDistanceBetweenSpawns = 300.0f;  // 최소 거리 설정
	
	for (int32 i = 0; i < NumberOfActorsToSpawn; ++i)
	{
		FVector SpawnLocation;
		bool bValidLocationFound = false;
	
		for (int32 Attempts = 0; Attempts < 100; ++Attempts)  // 충분히 시도
		{
			FVector RandomLocation = PlayerLocation + FMath::VRand() * FMath::FRandRange(0.0f, SpawnRadius);
			FVector StartLocation = RandomLocation + FVector(0, 0, 500.0f); // Start above the ground
			FVector EndLocation = RandomLocation - FVector(0, 0, 1000.0f); // End below the ground
	
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(AIPawn);
	
			bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	
			if (bHit)
			{
				SpawnLocation = HitResult.Location;
	
				// Check if the location is far enough from existing spawn locations
				bool bTooClose = false;
				for (const FVector& ExistingLocation : SpawnLocations)
				{
					if (FVector::Dist(ExistingLocation, SpawnLocation) < MinDistanceBetweenSpawns)
					{
						bTooClose = true;
						break;
					}
				}
	
				if (!bTooClose)
				{
					bValidLocationFound = true;
					break;
				}
			}
		}
		if (bValidLocationFound)
		{
			SpawnLocations.Add(SpawnLocation);
	
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = AIPawn;
			World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
}
