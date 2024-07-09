// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

// 무기는 --> 필요한 부서에 장착시킨다
// CAttachment를 상속받은 BP_Sword를 만들고
// 그블프에다가 무기를 붙여  놓고, 여기에서제공 AttachTo()함수를
// 이용하여  메쉬에 등록

// Collision관련 델리게이터
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);


UCLASS()
class U06_BATTLE_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
        class USceneComponent* Root;


public:       
    UFUNCTION(BlueprintNativeEvent)     
        void OnBeginEquip();                 // Call User defined C++ Source
    virtual void OnBeginEquip_Implementation() {};   // C에서는 정의 
    UFUNCTION(BlueprintNativeEvent)
        void OnEndEquip();
    virtual    void OnEndEquip_Implementation() {};
    UFUNCTION(BlueprintNativeEvent)
        void OnUnEquip();
     virtual void OnUnEquip_Implementation() {};

public:
    void  OnCollisions();
    void  OffCollisions();

public:  //델리게이터
    FAttachmentBeginCollision  OnAttachmentBeginCollision;
    FAttachmentEndCollision    OnAttachmentEndCollision;
    FAttachmentBeginOverlap    OnAttachmentBeginOverlap;
    FAttachmentEndOverlap      OnAttachmentEndOverlap;

private:
    UFUNCTION()
        void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
       void  OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





// BP에 제공할 함수 define
protected:
    UFUNCTION(BlueprintCallable, Category = "Attach")  // Sword,Hammer
        void  AttachTo(FName InSocketName);
    UFUNCTION(BlueprintCallable, Category = "Attach")  // Fist
        void  AttachToCollision(FName InSocketName);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Game")
        class ACharacter* OwnerCharacter;
    UPROPERTY(BlueprintReadOnly, Category = "Game")
        TArray<class UShapeComponent*> Collisions;


protected:
    class UCMovementComponent* Movement;
    class UCStateComponent*    State;

};
