// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

//***********************************************************
// 기존BP에서는 Weapons : TSubofclass
//                       Weapon->Weapon_Combo->Combo_Sword
// WeaponComponent
//   |-- WeaponAsset
//         |-- Attachment(Actor)  : BP화 Mesh와 Collider
//         |-- Equipment(Object)  : 장착관련 
//         |-- DoAction           : Combo
//         |-- SubDoAction        : 마우스클릭
//***********************************************************
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Fist,Sword,Hammer,Warp,Around,Bow,Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
   UPROPERTY(EditAnywhere, Category = "DataAsset")
        class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

   
public:
    UFUNCTION(BlueprintCallable)
        void SubAction_Pressed();
    UFUNCTION(BlueprintCallable)
        void SubAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void ZKeyAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void ZKeyAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void XKeyAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void XKeyAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void QKeyAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void QKeyAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void CKeyAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void CKeyAction_Released();

public:
    void SetUnarmedMode();
    void SetFistMode();
    void SetSwordMode();
    void SetHammerMode();
    void SetWarpMode();
    void SetAroundMode();
    void SetBowMode();

    void DoAction();

public:
	void DestoryWeapon();

private:
    bool IsIdleMode();
    void SetMode(EWeaponType InType);
    void ChangeType(EWeaponType InType);

public:
    class ACAttachment* GetAttachment();
    class UCEquipment*  GetEquipment();
    class UCDoAction*   GetDoAction();
    class UCSubAction*  GetSubAction();
	class UCZAction*	GetZKeyAction();
	class UCXAction*	GetXKeyAction();
	class UCCAction*	GetCKeyAction();
	class UCDownAttack*	GetQKeyAction();


private:
    class ACharacter* OwnerCharacter;          // 각 콤포넌트마다 생성시 변수를 저장
    EWeaponType  Type = EWeaponType::Max;

public:
    FWeaponTypeChanged  OnWeaponTypeChanged;   // 호출 : ABP,Player,Enemy
private:
    UPROPERTY()  // WeaponAsset에서 생성된 클래스를 려기에 저장
        class UCWeaponData* Datas[(int32)EWeaponType::Max];


public:
    FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
    FORCEINLINE bool IsFistMode()    { return Type == EWeaponType::Fist; }
    FORCEINLINE bool IsSwordMode()   { return Type == EWeaponType::Sword; }
    FORCEINLINE bool IsHammerMode()  { return Type == EWeaponType::Hammer; }
    FORCEINLINE bool IsWarpMode()    { return Type == EWeaponType::Warp; }
    FORCEINLINE bool IsAroundMode()  { return Type == EWeaponType::Around; }
    FORCEINLINE bool IsBowMode()     { return Type == EWeaponType::Bow; }


public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }
///////////// Boss Block
private:
	void PerformAttack();

	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackRange;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<class UDamageType> DamageTypeClass;
};

