// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_ClearUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/CSTutorialHUD.h"
void UCUserWidget_ClearUI::NativeConstruct()
{
	Super::NativeConstruct();

	RetryButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("RetryButton")));
	ExitButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
	if (RetryButtonUI)
	{
		RetryButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_ClearUI::RetryButtonCallback);
	}
	if (ExitButtonUI)
	{
		ExitButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_ClearUI::ExitButtonCallback);
	}


}
void UCUserWidget_ClearUI::RetryButtonCallback()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ACSTutorialHUD* HUD = Cast<ACSTutorialHUD>(PC->GetHUD()))
		{
			HUD->RestartGame();
		}
	}
}

void UCUserWidget_ClearUI::ExitButtonCallback()
{
	TSubclassOf<UUserWidget> StartWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/StartUI.StartUI_C'"));
	if (StartWidgetClass)
	{
		UUserWidget* StartWidget = CreateWidget<UUserWidget>(GetWorld(), StartWidgetClass);
		if (StartWidget)
		{
			StartWidget->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(StartWidget->TakeWidget()));
			}
		}
	}
}
