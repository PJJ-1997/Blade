// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Boss/CBoss_AI.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"
#include "Characters/CPlayerController.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Widgets/CUserWidget_Label.h"
#include "World/Pickup.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "ItemData/ItemBase.h"


ACBoss_AI::ACBoss_AI()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create Components
	{
		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
		CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	}
	AIControllerClass = ACAIController_Boss::StaticClass();
}

void ACBoss_AI::BeginPlay()
{
	Super::BeginPlay();

	UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/Widgets/WB_BossHealth.WB_BossHealth_C'"));
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	if (Widget)
	{
		Widget->AddToViewport();
		BossHPWidget = Cast<UCUserWidget_Label>(Widget);
	}
	//Widget
	if (BossHPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth());
	}
}

void ACBoss_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BossHPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth());
	}

}

void ACBoss_AI::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	BossAIController = Cast<ACAIController_Boss>(NewController);
	if (BossAIController)
	{
		if (BossHPWidget)
		{
			UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth());
		}
		BossAIController->StopAI();
	}
}

void ACBoss_AI::Hit()
{
	Super::Hit();
	CheckTrue(State->IsDeadMode());
	Behavior->SetHittedMode();
}

void ACBoss_AI::End_Hit()
{
	Super::End_Hit();
	Behavior->SetWaitMode();
}

void ACBoss_AI::Dead()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->GlobalAnimRateScale = 0.f;
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//
	//FVector start = GetActorLocation();
	//FVector target = Damage.Character->GetActorLocation();
	//FVector direction = target - start;
	//direction.Normalize();
	//
	//GetMesh()->AddForce(-direction * 2500000); --> ·¹±×µ¹

	Super::Dead();
	Behavior->SetDeadMode();
	if (BossAIController)
	{
		BossAIController->StopAI();
		UE_LOG(LogTemp, Warning, TEXT("Boss AI stopped because boss died"));
	}
}

void ACBoss_AI::End_Dead()
{
	//Destroy();
	Weapon->DestoryWeapon();
	Super::End_Dead();
}


void ACBoss_AI::UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth)
{
	if (BossHPWidget)
	{
		BossHPWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	}
}

void ACBoss_AI::HideWidgets()
{
	if (BossHPWidget)
	{
		BossHPWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (BossAIController)
	{
		BossAIController->StopAI();
	}
}

void ACBoss_AI::ShowWidgets()
{
	if (BossHPWidget)
	{
		BossHPWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (BossAIController)
	{
		BossAIController->StartAI();
	}
}