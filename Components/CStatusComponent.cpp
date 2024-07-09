
#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"


UCStatusComponent::UCStatusComponent()
{
	Health = MaxHealth; // �ʱ�ȭ
	HealSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundCue'/Game/Assets154/Assets154/Content/Ultimate_SFX/Ultimate_Water_Sounds/Cues/Drinking_Water_4_Cue.Drinking_Water_4_Cue'"));
	HealSound->bAutoActivate = false; // �ʱ�ȭ �� �ڵ����� ������� �ʵ��� ����
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    Health = MaxHealth;
	
}

void UCStatusComponent::Damage(float InAmount)
{
    Health += (InAmount*-1.0f);
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetHealth(float InHealth)
{
	Health = FMath::Clamp(InHealth, 0.0f, MaxHealth);
}

void UCStatusComponent::Heal(float InAmount)
{
	Health = FMath::Clamp(Health + InAmount, 0.0f, MaxHealth);

	if (HealSound != nullptr && HealSoundCue != nullptr)
	{
		// �Ҹ��� �����ϰ� ���
		HealSound->SetSound(HealSoundCue);
		HealSound->Play();
	}
}

