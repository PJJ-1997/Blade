
#include "Components/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

// #define LOG_UCFeetComponent

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float    leftDistance, rightDistance;
	FRotator leftRotator, rightRotator;

	Trace(LeftSocketName, leftDistance, leftRotator);
	Trace(RightSocketName, rightDistance, rightRotator);

	// 거리계산를 계산하여 최소값
	float offset = FMath::Min(leftDistance, rightDistance);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	// right의 경우는 X축(Z방향)
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -1.0f*(rightDistance - offset), DeltaTime, InterpSpeed);
	Data.LeftRotation = leftRotator;
	Data.RightRotation = rightRotator;

#ifdef LOG_UCFeetComponent
	CLog::Print(Data.LeftDistance, 11);
	CLog::Print(Data.RightDistance, 12);
	CLog::Print(Data.PelvisDistance, 13);
	CLog::Print(Data.LeftRotation, 14);
	CLog::Print(Data.RightRotation, 15);
#endif

}

void UCFeetComponent::Trace(FName InName, float & OutDistance, FRotator & OutRotation)
{
	FVector  socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float    z_start = OwnerCharacter->GetActorLocation().Z;  // 허리부분
	FVector  start = FVector(socket.X, socket.Y, z_start);    // 시작점
	
	float    z_end = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  end = FVector(socket.X, socket.Y, z_end);        // 끝점

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	// TraceTypeQuery1 : Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, DrawDebug, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	CheckFalse(hitResult.bBlockingHit);

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();  //지면보다 약 50아래
	OutDistance = length + OffsetDistance - TraceDistance;               // 5정도 +로 되어있음

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z) * -1.0f;


	OutRotation = FRotator(pitch, 0, roll);
}

