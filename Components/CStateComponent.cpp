#include "Components/CStateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame에서 Parent를 가져 올수 있음 : 생성자(X)
    OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCStateComponent::SetIdleMode()
{
    ChangeType(EStateType::Idle);
}

void UCStateComponent::SetBackStepMode()
{
    ChangeType(EStateType::BackStep);
}
void UCStateComponent::SetRollMode()
{
    ChangeType(EStateType::Roll);
}

void UCStateComponent::SetEquipMode()
{
    ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHitMode()
{
    ChangeType(EStateType::Hit);
}

void UCStateComponent::SetActionMode()
{
    ChangeType(EStateType::Action);
}

void UCStateComponent::SetDeadMode()
{
    ChangeType(EStateType::Dead);
}
// Noti에서도 처리가능하게 
void UCStateComponent::OnSubActionMode()
{
    bInSubActionMode = true;
}
void UCStateComponent::OffSubActionMode()
{
    bInSubActionMode = false;
}
void UCStateComponent::OnZKeyActionMode()
{
	bInZKeyActionMode = true;
}
void UCStateComponent::OffZKeyActionMode()
{
	bInZKeyActionMode = false;
}
void UCStateComponent::OnXKeyActionMode()
{
	bInXKeyActionMode = true;
}
void UCStateComponent::OffXKeyActionMode()
{
	bInXKeyActionMode = false;
}
void UCStateComponent::OnCKeyActionMode()
{
	bInCKeyActionMode = true;
}
void UCStateComponent::OffCKeyActionMode()
{
	bInCKeyActionMode = false;
}
void UCStateComponent::OnQKeyActionMode()
{
	bInQKeyActionMode = true;
}
void UCStateComponent::OffQKeyActionMode()
{
	bInQKeyActionMode = false;
}
void UCStateComponent::ChangeType(EStateType InType)
{
    EStateType prevType = Type;
    Type = InType;
    if (OnStateTypeChanged.IsBound())
        OnStateTypeChanged.Broadcast(prevType, Type);


}
