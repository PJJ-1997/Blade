// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

// ����� --> �ʿ��� �μ��� ������Ų��
// CAttachment�� ��ӹ��� BP_Sword�� �����
// �׺������ٰ� ���⸦ �ٿ�  ����, ���⿡������ AttachTo()�Լ���
// �̿��Ͽ�  �޽��� ���

// Collision���� ����������
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
    virtual void OnBeginEquip_Implementation() {};   // C������ ���� 
    UFUNCTION(BlueprintNativeEvent)
        void OnEndEquip();
    virtual    void OnEndEquip_Implementation() {};
    UFUNCTION(BlueprintNativeEvent)
        void OnUnEquip();
     virtual void OnUnEquip_Implementation() {};

public:
    void  OnCollisions();
    void  OffCollisions();

public:  //����������
    FAttachmentBeginCollision  OnAttachmentBeginCollision;
    FAttachmentEndCollision    OnAttachmentEndCollision;
    FAttachmentBeginOverlap    OnAttachmentBeginOverlap;
    FAttachmentEndOverlap      OnAttachmentEndOverlap;

private:
    UFUNCTION()
        void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
       void  OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





// BP�� ������ �Լ� define
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
