// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CSTutorialHUD.generated.h"

struct FInteractionData;
struct FInteractableData;
class  UInteractionWidget;
class  UMainMenu;
class  UCUserWidget_StartUI;
class  UCUserWidget_EndGame;
class  UCUserWidget_ClearUI;

UCLASS()
class U06_BATTLE_API ACSTutorialHUD : public AHUD
{
	GENERATED_BODY()
	//=========================================================//
	// PROPERTIES & VARIABLES
	//=========================================================//
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UMainMenu> MainMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	bool bIsMenuVisible;
	//=========================================================//
	// FUNCTIONS
	//=========================================================//
	ACSTutorialHUD();
	
	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget (const FInteractableData* InteractableData) const;
	void SetInputModeUIOnly();
protected:
	//=========================================================//
	// PROPERTIES & VARIABLES
	//=========================================================//
	UPROPERTY()
		UMainMenu* MainMenuWidget;
	UPROPERTY()
		UInteractionWidget* InteractionWidget;
	//=========================================================//
	// FUNCTIONS
	//=========================================================//
	virtual void BeginPlay() override;
/////////////////////////////////////
//StartUI
private:
	TSubclassOf<UUserWidget> StartLevelWidgetClass;
	class UCUserWidget_StartUI* StartLevelWidget;
//PauseUI
public:
	void TogglePauseMenu();
private:
	TSubclassOf<class UUserWidget> PauseMenuClass;
	class UCUserWidget_PauseMenu* PauseMenuWidget;
//EndUI
private:
	TSubclassOf<UUserWidget> EndGameWidgetClass;
	class UCUserWidget_EndGame* EndGameWidget;
//ClearUI
	TSubclassOf<UUserWidget> ClearGameWidgetClass;
	class UCUserWidget_ClearUI* ClearGameWidget;
//PlayerUI
public:
	void ShowEndGameWidget();
	void ShowClearGameWidget();
	void RestartGame();
};
