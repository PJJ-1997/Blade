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
    // Hit�� �����͸� Hit�迭�� ����,
    virtual void OnAttachmentEndCollision()   override; 
    // Hit�� �����͸� Hit�迭�� ���� :  EndOveralp()�� ó���Ҷ� ������ �����Ǵ� ����
    virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackerCauser, class ACharacter* InOther) override;
    
    virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override {};



private:
    int32 Index   =  0;       // �޺����ݽ��� �ε���
    bool  bEnable = false;    // �޺����������� ����
    bool  bExist  = false;    // �޺��� �ִ���?
    TArray<class ACharacter*> Hit;


public:
    FORCEINLINE void EnableCombo()  { bEnable = true; }
    FORCEINLINE void DisableCombo() { bEnable = false; }
};
