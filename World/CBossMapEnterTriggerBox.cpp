// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CBossMapEnterTriggerBox.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/CUserWidget_NeedItem.h"
#include "Kismet/GameplayStatics.h"

ACBossMapEnterTriggerBox::ACBossMapEnterTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ACBossMapEnterTriggerBox::OnOverlapBegin);
}
void ACBossMapEnterTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACBossMapEnterTriggerBox::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (OtherActor == PlayerCharacter && DoesPlayerHaveRequiredItem(OtherActor))
		{
			ACPlayer* Player = Cast<ACPlayer>(PlayerCharacter);
			if (Player)
			{
				Player->SavePlayerData();
			}
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossMap"));
		}
		else
		{
			ShowNeedItemWidget();
		}
	}
}

void ACBossMapEnterTriggerBox::ShowNeedItemWidget()
{
	if (!NeedItemWidgetClass) return;

	UCUserWidget_NeedItem* NeedItemWidget = CreateWidget<UCUserWidget_NeedItem>(GetWorld(), NeedItemWidgetClass);
	if (NeedItemWidget)
	{
		NeedItemWidget->AddToViewport();

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			PC->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}

bool ACBossMapEnterTriggerBox::DoesPlayerHaveRequiredItem(AActor * PlayerActor)
{
	ACPlayer* Player = Cast<ACPlayer>(PlayerActor);
	if (Player && Player->GetInventory())
	{
		return Player->GetInventory()->HasItemWithID("test_003");
	}
	return false;
}
