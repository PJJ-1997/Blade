// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CZAction_Bow.h"
#include "Widgets/CUserWidget_BowSkillUI.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void UCZAction_Bow::Pressed()
{
	Super::Pressed();
	ShowBowWidget();
	EnableMouseControl();
}

void UCZAction_Bow::ShowBowWidget()
{
	if (BowWidgetClass && !BowWidgetInstance)
	{
		BowWidgetInstance = CreateWidget<UCUserWidget_BowSkillUI>(Owner->GetWorld(), BowWidgetClass);
		if (BowWidgetInstance)
		{
			BowWidgetInstance->SetVisibility(ESlateVisibility::Visible); // Ensure visibility is set
			BowWidgetInstance->AddToViewport();
			BowWidgetInstance->SetActionBow(this);
			EnableMouseControl();
		}
	}
	else if (BowWidgetInstance)
	{
		BowWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		BowWidgetInstance->SetActionBow(this);
		EnableMouseControl();
	}
}

void UCZAction_Bow::HideBowWidget()
{
	if (BowWidgetInstance && BowWidgetInstance->Visibility == ESlateVisibility::Visible)
	{
		BowWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		DisableMouseControl();
	}
}

void UCZAction_Bow::EnableMouseControl()
{
	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(BowWidgetInstance->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void UCZAction_Bow::DisableMouseControl()
{
	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
