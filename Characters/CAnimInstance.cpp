#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CFeetComponent.h"
#include "Weapons/CSubAction.h"


//#include "Weapons/CSubAction.h"
//#include "Parkour/CParkourComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
    CheckNull(OwnerCharacter);

    // 바인딩하기위해
    // 
    // DECLARE_EVENT(UCAnimInstance, test);                       // Class내에서만
    // DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(Ftest,int32,A);  --> NineParam
    // 틀린점 : OnWeaponTypeChanged.Broadcast(prevType, InType);
    //         --> Broadcast를 어디에서든지 Call할수 있는게 DECLARE_DYNAMIC_MULTICAST_DELEGATE
    //         --> DECLARE_EVENT는 Class내에서만 BroadCast를 Call가능
    Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
 
    if (!!Weapon)
    {
        Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
    //   EWeaponType prevType = EWeaponType::Bow;
    //   EWeaponType inType = EWeaponType::Bow;
    //   if (Weapon->OnWeaponTypeChanged.IsBound())
    //       Weapon->OnWeaponTypeChanged.Broadcast(prevType, inType);
    }


}
void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    CheckNull(OwnerCharacter);

    Speed = OwnerCharacter->GetVelocity().Size2D();
   // Old Type:  Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());

    FRotator rotatorCharacter = OwnerCharacter->GetVelocity().ToOrientationRotator();
    FRotator rotatorControl   = OwnerCharacter->GetActorRotation();
    FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotatorCharacter, rotatorControl);
    PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);

    Direction = PrevRotation.Yaw;

    Pitch = UKismetMathLibrary::FInterpTo(Pitch,OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

    bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();


    // Bow
	CheckNull(Weapon);
	bBow_Aiming = true;
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming &= (WeaponType == EWeaponType::Bow);
		bBow_Aiming &= (Weapon->GetSubAction()->GetInAction());
	}

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent*   feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	// IK 할때 처리 : 파쿠르,FEET --> Player
	//CheckFalse(Weapon->IsUnarmedMode())

	bFeet = false;
	if (!!parkour && !!feet)
	{
		bFeet = parkour->IsExecuting() ? false : true;
		FeetData = feet->GetData();

	}

	else if (!!feet)
	{
		bFeet = true;
		FeetData = feet->GetData();
	}



}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
    WeaponType = InNewType;
}

