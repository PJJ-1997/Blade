// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"


UCLASS(Blueprintable)
class U06_BATTLE_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
	
public:
    void DoAction()       override;         
    void Begin_DoAction() override;
    void End_DoAction()   override; 
    
public:  // Collision
    virtual void OnAttachmentBeginCollision() override {};
    // Hit된 데이터를 Hit배열에 지움,
    virtual void OnAttachmentEndCollision()   override; 
    // Hit된 데이터를 Hit배열에 저장 :  EndOveralp()에 처리할때 굉장히 지연되는 현상
    virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackerCauser, class ACharacter* InOther) override;
    
    virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override {};



private:
    int32 Index   =  0;       // 콤보공격시의 인덱스
    bool  bEnable = false;    // 콤보가능한지의 변수
    bool  bExist  = false;    // 콤보가 있는지?
    TArray<class ACharacter*> Hit;


public:
    FORCEINLINE void EnableCombo()  { bEnable = true; }
    FORCEINLINE void DisableCombo() { bEnable = false; }
};
