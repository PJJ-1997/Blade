// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UCUserWidget_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �������Ʈ�� ��ư�� �̸��� ���� �����´�
	ResumeButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("RESUME")));
	ExitButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("EXIT")));

	// ��ư Ŭ���� ȣ��� ��������Ʈ�� �Լ��� ����Ѵ�
	ResumeButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_PauseMenu::ResumeGame);
	ExitButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_PauseMenu::QuitGame);
}
void UCUserWidget_PauseMenu::ResumeGame()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetPause(false);
		this->RemoveFromViewport();
		//FInputModeGameOnly InputMode;
		//PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

void UCUserWidget_PauseMenu::QuitGame()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("Start")));
}
