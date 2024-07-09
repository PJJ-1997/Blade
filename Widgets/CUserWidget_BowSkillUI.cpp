// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_BowSkillUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Weapons/SkillActions/CZAction_Bow.h"
#include "Weapons/AddOns/CArrow.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

void UCUserWidget_BowSkillUI::SetActionBow(UCZAction_Bow * NewActionBow)
{
	ActionBow = NewActionBow;
}

void UCUserWidget_BowSkillUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (NormalUI)
	{
		NormalUI->SetIsEnabled(true);  // 버튼 활성화
		NormalUI->SetVisibility(ESlateVisibility::Visible);
		NormalUI->OnClicked.AddDynamic(this, &UCUserWidget_BowSkillUI::NormalCallback);
	}
	if (FireUI)
	{
		FireUI->SetIsEnabled(true);  // 버튼 활성화
		FireUI->SetVisibility(ESlateVisibility::Visible);
		FireUI->OnClicked.AddDynamic(this, &UCUserWidget_BowSkillUI::FireCallback);
	}
	if (IceUI)
	{
		IceUI->SetIsEnabled(true);  // 버튼 활성화
		IceUI->SetVisibility(ESlateVisibility::Visible);
		IceUI->OnClicked.AddDynamic(this, &UCUserWidget_BowSkillUI::IceCallback);
	}
}

void UCUserWidget_BowSkillUI::NormalCallback()
{
	SetArrowEffect(NormalEffect, EArrowType::Normal);
	if (ActionBow)
	{
		ActionBow->HideBowWidget();
	}
}
void UCUserWidget_BowSkillUI::FireCallback()
{
	SetArrowEffect(RedEffect, EArrowType::Fire);
	if (ActionBow)
	{
		ActionBow->HideBowWidget();
	}
}
void UCUserWidget_BowSkillUI::IceCallback()
{
	SetArrowEffect(BlueEffect, EArrowType::Ice);
	if (ActionBow)
	{
		ActionBow->HideBowWidget();
	}
}

void UCUserWidget_BowSkillUI::SetArrowEffect(UNiagaraSystem * Effect, EArrowType ArrowType)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACArrow::StaticClass(), FoundActors);

			for (AActor* Actor : FoundActors)
			{
				ACArrow* Arrow = Cast<ACArrow>(Actor);
				if (Arrow && !Arrow->bHasBeenShot)  // 발사되지 않은 화살만 처리
				{
					Arrow->SetNiagaraEffect(Effect);
					Arrow->SetArrowType(ArrowType);
				}
			}
		}
	}
}
