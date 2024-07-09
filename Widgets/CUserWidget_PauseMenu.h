// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCUserWidget_PauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* ResumeButtonUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* ExitButtonUI;
public:
	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
		void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
		void QuitGame();
};
