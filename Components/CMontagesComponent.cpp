#include "Components/CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{
	// �ʱ�ȭ
   // �ε��ؾ� �� ��Ÿ�� �ּ� ��θ� �����մϴ�.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FKeyMontageAsset(TEXT("AnimMontage'/Game/Player/Animation/Montages/Sword/Frank_RPG_Katana_S2_Skill02_Montage.Frank_RPG_Katana_S2_Skill02_Montage'"));
	if (FKeyMontageAsset.Succeeded())
	{
		FKeyMontage = FKeyMontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BlockMontageAsset(TEXT("AnimMontage'/Game/Player/Animation/Montages/Common/Anim_K_BlockAim_Montage.Anim_K_BlockAim_Montage'"));
	if (BlockMontageAsset.Succeeded())
	{
		BlockMontage = BlockMontageAsset.Object;
	}
}


// Called when the game starts
void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

    if (DataTable == nullptr)
    {
        CLog::Warning(" Data table not defined!!!");
        return;
    }
    // InGame���� Parent�� ���� �ü� ���� : ������(X)
    OwnerCharacter = Cast<ACharacter>(GetOwner());
	
    TArray<FMontagesData*> OutRowArray;
    DataTable->GetAllRows<FMontagesData>("", OutRowArray);

	//Binding about Montage
	//OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCMontagesComponent::OnMontageEnded);
	//OwnerCharacter->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UCMontagesComponent::OnMontageNotifyBegin);

    if (0)
    {
        for (FMontagesData* data : OutRowArray)
        {
            if (data == nullptr)
                continue;
            FString str;
            str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
            str.Append(" / ");
            str.Append(data->Montage->GetPathName());
            CLog::Log(str);

        }
    }
  
    // Datas[]�� Type���� ����
    for (int32 i = 0; i < (int32)EStateType::Max; i++)
    {
        for (FMontagesData* data : OutRowArray)
        {
            if ((EStateType)i == data->Type)
            {
                Datas[i] = data;
            }

        }

    }
}
//void UCMontagesComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload & BranchingPointPayload)
//{
//	//FString str = NotifyName.ToString();
//	//CLog::Print(str);
//}
//void UCMontagesComponent::OnMontageEnded(UAnimMontage * Montage, bool bInterrupted)
//{
//	//if (bInterrupted)
//	//	CLog::Print("Interrupted Ended");
//	//else
//	//	CLog::Print("Normal Ended");
//}
void UCMontagesComponent::PlayBackStepMode()
{
    PlayAnimMontage(EStateType::BackStep);
}
void UCMontagesComponent::PlayRollMode()
{
    PlayAnimMontage(EStateType::Roll);
}
void UCMontagesComponent::PlayDeadMode()
{
    PlayAnimMontage(EStateType::Dead);
}
void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
    CheckNull(OwnerCharacter);
    FMontagesData* data = Datas[(int32)InType];
    if (data == nullptr)
    {
        return;
    }

    OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	//OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(data->Montage, data->PlayRate);

}

void UCMontagesComponent::PlayFKeyMontage()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		// ĳ���� �������� �޽��� �����ɴϴ�.
		USkeletalMeshComponent* Mesh = CharacterOwner->GetMesh();

		// UAnimInstance�� �����ɴϴ�.
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

		// �ִϸ��̼��� ����մϴ�.
		if (AnimInstance && FKeyMontage)
		{
			AnimInstance->Montage_Play(FKeyMontage);
		}
	}
}

void UCMontagesComponent::PlayBlockAnimation()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		// ĳ���� �������� �޽��� �����ɴϴ�.
		USkeletalMeshComponent* Mesh = CharacterOwner->GetMesh();

		// UAnimInstance�� �����ɴϴ�.
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

		// �ִϸ��̼��� ����մϴ�.
		if (AnimInstance && BlockMontage)
		{
			AnimInstance->Montage_Play(BlockMontage);
		}
	}
}
