// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/CSTutorialHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Boss/CBoss_AI.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/CUserWidget_StartUI.h"
#include "Widgets/CUserWidget_PauseMenu.h"
#include "Widgets/CUserWidget_Player.h"
#include "Widgets/CUserWidget_EndGame.h"
#include "Widgets/CUserWidget_ClearUI.h"
#include "Kismet/GameplayStatics.h"

ACSTutorialHUD::ACSTutorialHUD()
{
	static ConstructorHelpers::FClassFinder<UCUserWidget_StartUI> StartWidgetClass(TEXT("WidgetBlueprint'/Game/Widgets/StartUI.StartUI_C'"));

	if (StartWidgetClass.Succeeded())
		StartLevelWidgetClass = StartWidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UCUserWidget_EndGame> EndWidgetClass(TEXT("WidgetBlueprint'/Game/Widgets/EndUI.EndUI_C'"));

	if (EndWidgetClass.Succeeded())
		EndGameWidgetClass = EndWidgetClass.Class;
	
	static ConstructorHelpers::FClassFinder<UCUserWidget_ClearUI> ClearWidgetClass(TEXT("WidgetBlueprint'/Game/Widgets/ClearUI.ClearUI_C'"));

	if (ClearWidgetClass.Succeeded())
		ClearGameWidgetClass = ClearWidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("WidgetBlueprint'/Game/Widgets/PasueUI.PasueUI_C'"));
	if (PauseMenuBPClass.Class != nullptr)
	{
		PauseMenuClass = PauseMenuBPClass.Class;
	}
}

void ACSTutorialHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (IsValid(StartLevelWidget))
	{
		StartLevelWidget->RemoveFromParent();
		StartLevelWidget->Destruct();
		StartLevelWidget = nullptr;
	}
	// "" 해당맵에서는 위젯 제거
	FString CurrentMapName = GetWorld()->GetMapName();
	
	if (CurrentMapName.Contains(TEXT("Portal")))
	{
		return;
	}
	
	if (CurrentMapName.Contains(TEXT("BossMap")))
	{
		return; 
	}

	if (IsValid(StartLevelWidgetClass))
	{
		StartLevelWidget = Cast<UCUserWidget_StartUI>(CreateWidget(GetWorld(), StartLevelWidgetClass));

		if (IsValid(StartLevelWidget))
		{
			StartLevelWidget->AddToViewport();
			SetInputModeUIOnly(); // startWidget인 상태에서 키 입력 방지
		}
	}

	if (IsValid(EndGameWidgetClass))
	{
		EndGameWidget = Cast<UCUserWidget_EndGame>(CreateWidget(GetWorld(), EndGameWidgetClass));

		if (IsValid(EndGameWidget))
		{
			EndGameWidget->AddToViewport();
			EndGameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (IsValid(ClearGameWidgetClass))
	{
		ClearGameWidget = Cast<UCUserWidget_ClearUI>(CreateWidget(GetWorld(), ClearGameWidgetClass));

		if (IsValid(ClearGameWidget))
		{
			ClearGameWidget->AddToViewport();
			ClearGameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (IsValid(PauseMenuClass))
	{
		PauseMenuWidget = Cast<UCUserWidget_PauseMenu>(CreateWidget(GetWorld(), StartLevelWidgetClass));

		if (IsValid(PauseMenuWidget))
		{
			PauseMenuWidget->AddToViewport();
			GetOwningPlayerController()->SetShowMouseCursor(true);
		}
	}
}

void ACSTutorialHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACSTutorialHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ACSTutorialHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		// Player controller / mouse cursor
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}

}

void ACSTutorialHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACSTutorialHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACSTutorialHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}
			InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ACSTutorialHUD::SetInputModeUIOnly()
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(StartLevelWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void ACSTutorialHUD::TogglePauseMenu()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (PC->IsPaused())
		{
			PC->SetPause(false);
			if (PauseMenuWidget)
			{
				PauseMenuWidget->RemoveFromViewport();
				PauseMenuWidget = nullptr;
			}
			PC->bShowMouseCursor = false;
		}
		else
		{
			PC->SetPause(true);
			if (PauseMenuClass)
			{
				PauseMenuWidget = CreateWidget<UCUserWidget_PauseMenu>(PC, PauseMenuClass);
				if (PauseMenuWidget)
				{
					PauseMenuWidget->AddToViewport();
					PC->bShowMouseCursor = true;
				}
			}
		}
	}
}

void ACSTutorialHUD::ShowEndGameWidget()
{
	if (!IsValid(EndGameWidget) && IsValid(EndGameWidgetClass))
	{
		EndGameWidget = Cast<UCUserWidget_EndGame>(CreateWidget(GetWorld(), EndGameWidgetClass));
		if (IsValid(EndGameWidget))
		{
			EndGameWidget->AddToViewport();
		}
	}

	if (IsValid(EndGameWidget))
	{
		EndGameWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EndGameWidget is not valid."));
	}
}

void ACSTutorialHUD::ShowClearGameWidget()
{
	if (!IsValid(ClearGameWidget) && IsValid(ClearGameWidgetClass))
	{
		ClearGameWidget = Cast<UCUserWidget_ClearUI>(CreateWidget(GetWorld(), ClearGameWidgetClass));
		if (IsValid(ClearGameWidget))
		{
			ClearGameWidget->AddToViewport();
		}
	}

	if (IsValid(ClearGameWidget))
	{
		ClearGameWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClearGameWidget is not valid."));
	}
}

void ACSTutorialHUD::RestartGame()
{
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::OpenLevel(this, FName(TEXT("Start")));
}

