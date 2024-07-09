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

	// Player사용된것을 보관
	{
		SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera	  = CHelpers::GetComponent<UCameraComponent>(InOwner);
	}
	// Curve관련하여
	{
		FOnTimelineVector timeline;

		timeline.BindUFunction(this, "OnAming");  // Curve에서 변경된 Vector가 OnAming함수로
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


	// 현재카메라 데이터를 OriginData변수에 저장
	{
		OriginData.TargetArmLength = SpringArm->TargetArmLength;
		OriginData.SocketOffset = SpringArm->SocketOffset;
		OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
		OriginData.CameraLocation = Camera->GetRelativeLocation();

	}


	// AimData로 변경
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

	// 기존데이터로 변경
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
	// Output.X = (90~45)  FieldOfView값
   // Output.Y = (0-100)  Pitch값

	Camera->FieldOfView = Output.X;
	if (!!Bend)
		*Bend = Output.Y;

}
