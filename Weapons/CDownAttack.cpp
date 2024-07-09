// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDownAttack.h"
#include "Global.h"
#include "CDownAttack.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCDownAttack::UCDownAttack()
{

}
void UCDownAttack::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCDownAttack::Pressed()
{
	bInAction = true;
}

void UCDownAttack::Released()
{
	bInAction = false;
}
