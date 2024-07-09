// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_NeedItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/CSTutorialHUD.h"

void UCUserWidget_NeedItem::NativeConstruct()
{
	Super::NativeConstruct();

	CheckButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("CheckButton")));

	if (CheckButtonUI)
	{
		CheckButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_NeedItem::CheckButtonCallback);
	}

}
void UCUserWidget_NeedItem::CheckButtonCallback()
{
	RemoveFromParent();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

}
