// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_StartUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCUserWidget_StartUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	StartButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	EndButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("EndButton")));

	// 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
	StartButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_StartUI::StartButtonCallback);
	EndButtonUI->OnClicked.AddDynamic(this, &UCUserWidget_StartUI::EndButtonCallback);
}

void UCUserWidget_StartUI::StartButtonCallback()
{
	
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Portal"));
	
}

void UCUserWidget_StartUI::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
