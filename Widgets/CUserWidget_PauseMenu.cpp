// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UCUserWidget_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	ResumeButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("RESUME")));
	ExitButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("EXIT")));

	// 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
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
