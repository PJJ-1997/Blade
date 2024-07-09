// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CCAction.h"
#include "Global.h"
#include "CCAction.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCCAction::UCCAction()
{

}
void UCCAction::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCCAction::Pressed()
{
	bInAction = true;
}

void UCCAction::Released()
{
	bInAction = false;
}
