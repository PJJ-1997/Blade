// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "CSubAction_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
}

void UCSubAction_Bow::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	// Player���Ȱ��� ����
	{
		SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera	  = CHelpers::GetComponent<UCameraComponent>(InOwner);
	}
	// Curve�����Ͽ�
	{
		FOnTimelineVector timeline;

		timeline.BindUFunction(this, "OnAming");  // Curve���� ����� Vector�� OnAming�Լ���
		Timeline.AddInterpVector(Curve, timeline);
		Timeline.SetPlayRate(AimingSpeed);
	}

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
		Bend = bow->GetBend();
}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::Pressed()
{
	CheckTrue(State->IsSubActionMode());
	
	if (!!Owner->GetController<AAIController>())
	{
		Super::Pressed();          // bInAction = true;
		State->OnSubActionMode();  // bInSubActionMode = true;

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);
	
	Super::Pressed();

	State->OnSubActionMode();


	// ����ī�޶� �����͸� OriginData������ ����
	{
		OriginData.TargetArmLength = SpringArm->TargetArmLength;
		OriginData.SocketOffset = SpringArm->SocketOffset;
		OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
		OriginData.CameraLocation = Camera->GetRelativeLocation();

	}


	// AimData�� ����
	{
		SpringArm->TargetArmLength = 150.0f;
		SpringArm->SocketOffset = AimData.SocketOffset;
		SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
		Camera->SetRelativeLocation(AimData.CameraLocation);
	}
	

	Timeline.PlayFromStart();
}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());

	if (!!Owner->GetController<AAIController>())
	{
		Super::Released();          // bInAction = true;
		State->OffSubActionMode();  // bInSubActionMode = true;

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released(); //   bInAction = false;

	State->OffSubActionMode();

	// ���������ͷ� ����
	{
		SpringArm->TargetArmLength = OriginData.TargetArmLength;
		SpringArm->SocketOffset = OriginData.SocketOffset;
		SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
		Camera->SetRelativeLocation(OriginData.CameraLocation);
	}

	Timeline.ReverseFromEnd();
}

void UCSubAction_Bow::OnAming(FVector Output)
{
	// Output.X = (90~45)  FieldOfView��
   // Output.Y = (0-100)  Pitch��

	Camera->FieldOfView = Output.X;
	if (!!Bend)
		*Bend = Output.Y;

}
