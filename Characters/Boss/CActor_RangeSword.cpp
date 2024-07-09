#include "Characters/Boss/CActor_RangeSword.h"
#include "Global.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

ACActor_RangeSword::ACActor_RangeSword()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	this->InitialLifeSpan = 5.0f;

	// HitData정보
	{
		HitData.Launch = 0.0f;
		HitData.Power = 5.0f;
		CHelpers::GetAsset<UAnimMontage>(&HitData.Montage, "AnimMontage'/Game/ArashiAkuma/Animations/Montages/HitReaction_Player.HitReaction_Player'");
	}
}

void ACActor_RangeSword::BeginPlay()
{
	Super::BeginPlay();
	FindTarget();

	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACActor_RangeSword::OnComponentBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &ACActor_RangeSword::OnComponentEndOverlap);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACActor_RangeSword::SendMessage, DamageInterval, true);
	}

	//FVector StartLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 100);
	//
	//SetActorLocation(StartLocation);
}

void ACActor_RangeSword::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//return;
	//
	//while (1)
	//{
	//	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	//		break;
	//}
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ACActor_RangeSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Target == nullptr)
	//{
	//	FindTarget();
	//	if (Target == nullptr) return;  
	//}

	HomingTimeElapsed += DeltaTime;

	
	if (HomingTimeElapsed <= HomingDuration)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Normalize();

		FRotator TargetRotation = Direction.Rotation();
		FRotator CurrentRotation = GetActorRotation();

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotateSpeed);
		SetActorRotation(NewRotation);
	
	}
	else
	{
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(CurrentRotation);

		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);
		SetActorLocation(NewLocation);
	}

	//Super::Tick(DeltaTime);
	//
	//CheckNull(GetOwner());
	//if (Target == nullptr)
	//{
	//	FindTarget();
	//	if (Target == nullptr) return;  // 타겟을 찾지 못한 경우 업데이트 종료
	//}
	//
	//FVector Direction = Target->GetActorLocation() - GetActorLocation();
	//Direction.Normalize();
	//
	//FRotator TargetRotation = Direction.Rotation();
	//FRotator CurrentRotation = GetActorRotation();
	//
	////FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotateSpeed);
	////SetActorRotation(NewRotation);
	//
	//FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);
	//SetActorLocation(NewLocation);
}

void ACActor_RangeSword::FindTarget()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), EnemyTag, Enemies);

	float ShortestDistance = FLT_MAX;
	AActor* NearestEnemy = nullptr;

	for (AActor* Enemy : Enemies)
	{
		float DistanceToEnemy = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
		if (DistanceToEnemy < ShortestDistance)
		{
			ShortestDistance = DistanceToEnemy;
			NearestEnemy = Enemy;
		}
	}

	if (NearestEnemy != nullptr)
	{
		Target = NearestEnemy;
	}
}

void ACActor_RangeSword::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
}

void ACActor_RangeSword::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.AddUnique(chararcter);
}
void ACActor_RangeSword::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.Remove(chararcter);
}
void ACActor_RangeSword::SendMessage()
{
	if (Hit.Num() == 0)
		return;
	for (int32 i = Hit.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hit[i]);

	Destroy();
}