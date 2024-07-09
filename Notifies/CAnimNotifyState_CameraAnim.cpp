
#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraAnim.h"
#include "Weapons/AddOns/CCameraModifier.h"

FString UCAnimNotifyState_CameraAnim::GetNotifyName_Implementation() const
{
    return "CameraAnim";
}

void UCAnimNotifyState_CameraAnim::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    // ABP�����ؼ��� InGame & ABP ������ �Ǳ� ������ NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

	//if (!MeshComp || !MeshComp->GetOwner()) {
	//	UE_LOG(LogTemp, Error, TEXT("Error: MeshComp or MeshComp->GetOwner() is nullptr in NotifyBegin"));
	//	return;  // ���� ó�� �Ǵ� �Լ� ����
	//}
    CheckNull(CameraAnim);

	
	
    ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());

	//if (!character) {
	//	UE_LOG(LogTemp, Error, TEXT("Error: 'character' is nullptr in NotifyBegin"));
	//	return;  // ���� ó�� �Ǵ� �Լ� ����
	//}

    CheckNull(character);

	


    APlayerController* controller = character->GetController<APlayerController>();

	//if (!controller) {
	//	UE_LOG(LogTemp, Error, TEXT("Error: 'controller' is nullptr in NotifyBegin"));
	//	return;
	//}

	CheckNull(controller);

	

    UCameraModifier* modifier = controller->PlayerCameraManager->AddNewCameraModifier(UCCameraModifier::StaticClass());

    Modifier = Cast<UCCameraModifier>(modifier);

    ACharacter* target = character;
	CheckNull(target);
    // ���� SoftReference
    if (Target.IsNull() == false)
        target = Target.Get();

	if (!target) {
		UE_LOG(LogTemp, Error, TEXT("Error: 'target' is nullptr in NotifyBegin"));
		return;  // ���� ó�� �Ǵ� �Լ� ����
	}

    FVector location = target->GetActorLocation();
    float   yaw = controller->GetControlRotation().Yaw;


    location = location + FRotator(0, yaw, 0).RotateVector(LocationOffset);
    Modifier->SetLocation(location);

   //Modifier->SetRotation(FRotator(PitchOffset, yaw, 0));
   //controller->SetControlRotation(FRotator(0, controller->GetControlRotation().Yaw, 0));
	Modifier->SetRotation(FRotator(-PitchOffset, yaw+180.0f, 0));
	controller->SetControlRotation(FRotator(0, yaw + 180.0f, 0));

  //  PlayCameraAnim(FRotator UserPlaySpaceRot)

     // ī�޶� ������̾ �̿��Ͽ� 
    // ECameraShakePlaySpace::UserDefined --> ��ġ�� ��������
   
    controller->PlayerCameraManager->PlayCameraAnim(CameraAnim, PlayRatio,1, BlendInTime, BlendOutTime,false,false,
        0, ECameraShakePlaySpace::UserDefined, controller->GetControlRotation());



}
void UCAnimNotifyState_CameraAnim::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    // ABP�����ؼ��� InGame & ABP ������ �Ǳ� ������ NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());
    CheckNull(CameraAnim);

    ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
    CheckNull(character);

    APlayerController* controller = character->GetController<APlayerController>();
    CheckNull(controller);

    controller->PlayerCameraManager->RemoveCameraModifier(Modifier);

}