// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CEnemy_AI.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"
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


ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create Components
	{

		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
		CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	}
#if WITH_EDITOR
	{
		CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

		TSubclassOf<UCUserWidget_Label> labelClass;
		CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'");

		LabelWidget->SetWidgetClass(labelClass);
		LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
		LabelWidget->SetDrawSize(FVector2D(120, 0));
		LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
#endif


}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	LabelWidget->InitWidget();
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	label->UpdateName(GetName());
	label->UpdateControllerName(GetController()->GetName());

#endif
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	if (!!label)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		UpdateLabelRenderScale();
	}
}

void ACEnemy_AI::Hit()
{
	Super::Hit();
	CheckTrue(State->IsDeadMode());
	Behavior->SetHittedMode();
}

void ACEnemy_AI::End_Hit()
{
	Super::End_Hit();
	Behavior->SetWaitMode();
}

void ACEnemy_AI::Dead()
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
	//GetMesh()->AddForce(-direction * 2500000);

	//if (!Status->IsDead())
	//{
	//	FVector start = GetActorLocation();
	//	FVector target = Damage.Character->GetActorLocation();
	//	FVector direction = target - start;
	//
	//	direction = direction.GetSafeNormal();
	//	LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
	//	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	//}

	Super::Dead();
	Behavior->SetDeadMode();
}

void ACEnemy_AI::End_Dead()
{
	Destroy();
	Weapon->DestoryWeapon();
	DropItems();
	Super::End_Dead();
}

void ACEnemy_AI::UpdateLabelRenderScale()
{
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CheckNull(cameraManager);

	FVector camreaLocation = cameraManager->GetCameraLocation();    // Player
	FVector targetLocation = GetController()->GetTargetLocation();  // Enemy

	// 거리구하기
	float distance = FVector::Distance(camreaLocation, targetLocation);
	float sizeRate = 1.0f - (distance / LabelViewAmount);

	// ESlateVisibility : 항목을 정리
	if (distance > LabelViewAmount)
	{
		// 3000보다가 크면 label off
		// 또한, Collapsed는 레이아웃 공간을 차지하지 않아 숨겨진 후에 Prepass 계산을 무시하기 때문에 Hidden보다 성능이 우수하다고 할 수 있다.
		label->SetVisibility(ESlateVisibility::Collapsed);
		return;

	}
	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy_AI::DropItems()
{
	// 적이 죽을 때 호출되는 함수

	if (ItemDataTable && !DesiredItemID.IsNone() && DropItemClass)
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		if (ItemData)
		{
			// 아이템 드롭 로직
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();

			APickup* DroppedItem = GetWorld()->SpawnActor<APickup>(DropItemClass, SpawnLocation, SpawnRotation);
			if (DroppedItem)
			{
				UItemBase* ItemToDrop = NewObject<UItemBase>(DroppedItem, UItemBase::StaticClass());
				ItemToDrop->ID = ItemData->ID;
				ItemToDrop->ItemType = ItemData->ItemType;
				ItemToDrop->NumericData = ItemData->NumericData;
				ItemToDrop->TextData = ItemData->TextData;
				ItemToDrop->AssetData = ItemData->AssetData;

				//DroppedItem->InitializeDrop(ItemToDrop, ItemToDrop->NumericData.MaxStackSize);
				int32 QuantityToDrop = FMath::Max(1, DropItemQuantity);  // 최소 1개 이상 드롭
				DroppedItem->InitializeDrop(ItemToDrop, QuantityToDrop);
			}
		}
	}
}


