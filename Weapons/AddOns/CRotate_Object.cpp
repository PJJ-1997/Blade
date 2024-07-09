


#include "Weapons/AddOns/CRotate_Object.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACRotate_Object::ACRotate_Object()
{
	PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
    CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Capsule);

    Capsule->SetCapsuleHalfHeight(44);
    Capsule->SetCapsuleRadius(44);

    this->InitialLifeSpan = 5.0f;

    // HitData정보
    {
        HitData.Launch = 0.0f;
        HitData.Power  = 5.0f;
        CHelpers::GetAsset<UAnimMontage>(&HitData.Montage, "AnimMontage'/Game/Character/Montages/Common/HitReaction_Montage.HitReaction_Montage'");
    }
}


void ACRotate_Object::BeginPlay()
{
	Super::BeginPlay();

    // Angle값 Radomm으로 초기화

    Angle = UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f);

    // Binging : inGame
    {
        Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACRotate_Object::OnComponentBeginOverlap);
        Capsule->OnComponentEndOverlap.AddDynamic(this, &ACRotate_Object::OnComponentEndOverlap);

        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACRotate_Object::SendMessage, DamageInterval, true);
    }

}
// TimerHandle을 Destroy되기전에 지우기 위해
void ACRotate_Object::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    return;

    while (1)
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
            break;
    }
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ACRotate_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CheckNull(GetOwner());
    FVector location = GetOwner()->GetActorLocation();

    if (bNegative)
        Angle -= Speed * DeltaTime;
    else
        Angle += Speed * DeltaTime;

    if (FMath::IsNearlyEqual(Angle, bNegative ? -360 : 360))
        Angle = 0.0f;

    FVector distance = FVector(Distance, 0, 0);   
    FVector value = distance.RotateAngleAxis(Angle, FVector::UpVector);

    location += value;
    SetActorLocation(location);
    SetActorRotation(FRotator(0, Angle, 0));

}

void ACRotate_Object::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckTrue(GetOwner() == OtherActor);

    ACharacter* chararcter = Cast<ACharacter>(OtherActor);
    if (!!chararcter)
        Hit.AddUnique(chararcter);
}
void ACRotate_Object::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckTrue(GetOwner() == OtherActor);
    ACharacter* chararcter = Cast<ACharacter>(OtherActor);
    if (!!chararcter)
        Hit.Remove(chararcter);
}
void ACRotate_Object::SendMessage()
{
    if (Hit.Num() == 0)
        return;
    for (int32 i = Hit.Num() - 1; i >= 0; i--)
        HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hit[i]);
}