// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CZAction.h"
#include "CZAction_Bow.generated.h"

class UCUserWidget_BowSkillUI;

UCLASS()
class U06_BATTLE_API UCZAction_Bow : public UCZAction
{
	GENERATED_BODY()
public:
	virtual void Pressed() override;
public:
	void HideBowWidget();
private:
	void ShowBowWidget();
	void EnableMouseControl();
	void DisableMouseControl();

public:
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UCUserWidget_BowSkillUI> BowWidgetClass;

	UPROPERTY()
		class UCUserWidget_BowSkillUI* BowWidgetInstance;
};
