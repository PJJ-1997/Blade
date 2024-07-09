// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CSubAction.generated.h"

//Blueprintable
//UCLASS(Abstract)
UCLASS(Blueprintable)
class U06_BATTLE_API UCSubAction : public UObject
{
    GENERATED_BODY()

public:
	UCSubAction();
public:
    virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public: // 마우스
    virtual void Pressed();
    virtual void Released();

    // 블프에서 작성될 것들 --> 기술 면접시 질문
    /*
    BlueprintCallable
    --> C++ 로 작성됨.블루프린트 그래프에서 호출 가능하지만 변경이나 덮어쓰기는 불가능(e.g.수학 함수)

    BlueprintImplementableEvent
    --> 헤더 파일(.h) 에 추가되지만, 함수 본문은 C++ 가 아닌 블루프린트 그래프에서 작성됨.
        표준 동작이 없는 이벤트를 자유롭게 변경할 수 있도록 하는 경우에 사용. (e.g.각 보스마다 다른 보상을 줌)

    BlueprintNativeEvent
    --> BlueprintCallable 과 BlueprintImplementableEvent 의 조합형.C++ 로 작성되었지만, 블루프린트에서 보조 또는 대체 가능함.
        BlueprintNativeEvent 를 사용할 때는 함수 마지막에 "_Implementation" 을 붙여야 함.
    */


public:
    UFUNCTION(BlueprintNativeEvent)
        void Begin_SubAction();
    virtual void Begin_SubAction_Implementation() {};
    
    UFUNCTION(BlueprintNativeEvent)
        void End_SubAction();
    virtual void End_SubAction_Implementation() {};

    UFUNCTION(BlueprintNativeEvent)
        void Tick(float InDeltaTime);
    virtual void Tick_Implementation(float InDeltaTime) {};

protected:
    bool    bInAction = false; // action중인지 check flag
    UPROPERTY()
        class   ACharacter*         Owner;
    UPROPERTY()
        class   ACAttachment*      Attachment;
    UPROPERTY()
        class   UCDoAction*        DoAction;
    UPROPERTY()
        class   UCStateComponent*        State;
    UPROPERTY()
        class   UCMovementComponent*         Movement;

public:
    FORCEINLINE bool GetInAction() { return bInAction; }
};
