// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapons/SkillActions/CZAction_Bow.h"
#include "Weapons/AddOns/CArrow.h"
#include "CUserWidget_BowSkillUI.generated.h"


UCLASS()
class U06_BATTLE_API UCUserWidget_BowSkillUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetActionBow(UCZAction_Bow* NewActionBow);
private:
	UFUNCTION()
		void NormalCallback();
	UFUNCTION()
		void FireCallback();
	UFUNCTION()
		void IceCallback();

	UCZAction_Bow* ActionBow;

	void SetArrowEffect(UNiagaraSystem* Effect, EArrowType ArrowType);

protected:
	UPROPERTY(meta = (BindWidget))
		class UButton* NormalUI;

	UPROPERTY(meta = (BindWidget))
		class UButton* FireUI;

	UPROPERTY(meta = (BindWidget))
		class UButton* IceUI;

	UPROPERTY(EditAnywhere, Category = "Effects")
		class UNiagaraSystem* NormalEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
		class UNiagaraSystem* RedEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
		class UNiagaraSystem* BlueEffect;
};
