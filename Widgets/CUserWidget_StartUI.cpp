// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_StartUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCUserWidget_StartUI::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �������Ʈ�� ��ư�� �̸��� ���� �����´�
	StartButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	EndButtonUI = Cast<UButton>(GetWidgetFromName(TEXT("EndButton")));

	// ��ư Ŭ���� ȣ��� ��������Ʈ�� �Լ��� ����Ѵ�
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
