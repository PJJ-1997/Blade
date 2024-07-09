
#include "Components/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"

//#define LOG_UCParkourComponent

void FParkourData::PlayMontage(class ACharacter * InCharacter)
{
	if (bFixedCamera)
	{
		UCMovementComponent*  movement = CHelpers::GetComponent<UCMovementComponent>(InCharacter);
		if (!!movement)
			movement->EnableFixedCamera();
	}
	InCharacter->PlayAnimMontage(Montage, PlayRatio, SectionName);
}

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");
}


void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkourData*> datas;
	DataTable->GetAllRows<FParkourData>("", datas);

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;
		for (FParkourData* data : datas)
		{
			if (data->Type == (EParkourType)i)
				temp.Add(*data);
		}
		DataMap.Add((EParkourType)i, temp);
	}

	// Arrow Component
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	USceneComponent* arrow = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");

	TArray<USceneComponent*> components;
	arrow->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		Arrows[i] = Cast<UArrowComponent>(components[i]);
	}

}


void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTrace_Land();

	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	CheckTrace_Center();
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor     color = FLinearColor(arrow->ArrowColor);

	FTransform  transform = arrow->GetComponentToWorld();


	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector()*TraceDistance;

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3,
		false, ignores, DebugType, HitResults[(int32)InType], true, color, FLinearColor::White);

}

void UCParkourComponent::DoParkour(bool bLanded)
{

	CheckFalse(Type == EParkourType::Max);

	if (bLanded == true && Check_FallMode())
	{
		DoParkour_FallMode();
		return;
	}

	// 초기화
	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	CheckTrace_Center();

	if (!!HitObstacle)
	{
		CheckTrace_Ceil();
		CheckTrace_Floor();
		CheckTrace_LeftRight();
	}
	// 1. Player가 장애물의 모서리에 있지 않아야함
	// 2. Arrow의 결과값 ( Center,Left,Right)이 존재하여야 한다
	// 3. 장애물과 Player간의 Angle이 AvailableFronAngle(15도)보다 작아야 한다 
	CheckFalse(Check_Obstacle());

	if (Check_ClimbMode())
	{
		DoParkour_ClimbMode();
		return;
	}
	if (Check_SlideMode())
	{
		DoParkour_SlideMode();
		return;
	}

	FParkourData data;
	if (Check_ObstacleMode(EParkourType::Normal, data))
	{
		DoParkour_ObstacleMode(data);
		return;
	}

	if (Check_ObstacleMode(EParkourType::Short, data))
	{
		DoParkour_ObstacleMode(data);
		return;
	}

	if (Check_ObstacleMode(EParkourType::Wall, data))
	{
		DoParkour_ObstacleMode(data);
		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)
	{
	case EParkourType::Climb:
		End_DoParkour_ClimbMode();
		break;
	case EParkourType::Slide:
		End_DoParkour_SlideMode();
		break;
	case EParkourType::Normal:
	case EParkourType::Short:
	case EParkourType::Wall:
		End_DoParkour_ObstacleMode();
		break;
	}

	Type = EParkourType::Max;
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (!!movement)
		movement->DisableFixedCamera();
}

bool UCParkourComponent::Check_Obstacle()
{
	CheckNullResult(HitObstacle, false);

	//	1. Arrow의 결과값(Center, Left, Right)이 존재하여야 한다
	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;

	if (b == false)
		return false;

	// 2. 모서리 Check
	FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

	CheckFalseResult(center.Equals(left), false);
	CheckFalseResult(center.Equals(right), false);

	// 3. Angle 처리 

		// 각도값
	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();
	float   lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal; //면(법선),구/실린더(중심)
	float   impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float   yaw = abs(abs(lookAt) - abs(impactAt));
	CheckFalseResult(yaw <= AvailableFrontAngle, false);
	return true;
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);
	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Climb);

	// CLog::Print((*datas)[0].MinDistance);
	// CLog::Print((*datas)[0].MaxDistance);
	// CLog::Print((*datas)[0].Montage);

	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent, HitObstacleExtent.Z, 10), false);

	return true;
}

void UCParkourComponent::DoParkour_ClimbMode()
{
	Type = EParkourType::Climb;

	// 벽면에 달라붙게하기위해 위치 조정
	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);
	// 모서리부분에서 허공으로 가지 않게하기위해
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	// 몽타쥬 run
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::End_DoParkour_ClimbMode()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UCParkourComponent::Check_SlideMode()
{
	// hit된것 check
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Floor].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);

	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);


	// 발에 있는 Arrow
	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Floor];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	// LineTrace
	FTransform  transform = arrow->GetComponentToWorld();
	FVector     arrowLocation = transform.GetLocation();
	FVector     ownerLocation = OwnerCharacter->GetActorLocation();


	float extent = (*datas)[0].Extent;
	float z = arrowLocation.Z + extent;


	FVector forward = OwnerCharacter->GetActorForwardVector();
	forward = FVector(forward.X, forward.Y, 0.0f);

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);
	FVector end = start + (forward * TraceDistance);


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;

	// TraceChannel1 : Visibility
	// TraceChannel2 : Camera
	// TraceChannel3 : Parkour(ignore) 
	// Preset        : Parkour(block)
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end, FVector(0, extent, extent),
		OwnerCharacter->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false,
		ignores, DebugType, hitResult, true, color);

	CheckTrueResult(hitResult.bBlockingHit, false);
	return true;
}

void UCParkourComponent::DoParkour_SlideMode()
{
	Type = EParkourType::Slide;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	// Box의 CollisionOff
	BackUpObstacle = HitObstacle;
	BackUpObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_SlideMode()
{
	BackUpObstacle->SetActorEnableCollision(true);
	BackUpObstacle = NULL;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData & OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(InType);


	for (int32 i = 0; i < (*datas).Num(); i++)
	{
		bool b = true;
		b &= ((*datas)[i].MinDistance < HitDistance);
		b &= ((*datas)[i].MaxDistance > HitDistance);
		b &= FMath::IsNearlyEqual((*datas)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*datas)[i];
		CheckTrueResult(b, true);
	}

	return false;
}

void UCParkourComponent::DoParkour_ObstacleMode(FParkourData & InData)
{
	Type = InData.Type;
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	InData.PlayMontage(OwnerCharacter);

	// Box의 CollisionOff
	BackUpObstacle = HitObstacle;
	BackUpObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_ObstacleMode()
{
	BackUpObstacle->SetActorEnableCollision(true);
	BackUpObstacle = NULL;
}

bool UCParkourComponent::Check_FallMode()
{
	// 지금현재 falling중이면 return
	CheckFalseResult(bFalling, false);

	// 땅에도착시 bFalling이 true
	bFalling = false;

	float  distance = HitResults[(int32)EParkourArrowType::Land].Distance;
	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);


	CheckFalseResult((*datas)[0].MinDistance < distance, false);
	CheckFalseResult((*datas)[0].MaxDistance > distance, false);

	return true;
}

void UCParkourComponent::DoParkour_FallMode()
{
	Type = EParkourType::Fall;
	// 몽타쥬 run
	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	// Hit결과를 Check
	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);

	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);

	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);

	HitObstacleExtent = FVector(x, y, z);
	HitDistance = hitResult.Distance;
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;

#ifdef LOG_UCParkourComponent
	{
		CLog::Print(HitObstacle, 10);
		CLog::Print(HitObstacleExtent, 11);
		CLog::Print(HitDistance, 12);
		CLog::Print(ToFrontYaw, 13);
	}
#endif
}

void UCParkourComponent::CheckTrace_Ceil()
{
	EParkourArrowType type = EParkourArrowType::Ceil;
	LineTrace(type);
}

void UCParkourComponent::CheckTrace_Floor()
{
	EParkourArrowType type = EParkourArrowType::Floor;
	LineTrace(type);
}

void UCParkourComponent::CheckTrace_LeftRight()
{
	EParkourArrowType type = EParkourArrowType::Left;
	LineTrace(type);
	LineTrace(EParkourArrowType::Right);
}

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());
	CheckTrue(bFalling);
	bFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform  transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);

	FVector end = start + (transform.GetRotation().GetForwardVector()*(*datas)[0].Extent);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	// TraceChannel1 : Visibility
	// TraceChannel2 : Camera
	// TraceChannel3 : Parkour(ignore) 
	// Preset        : Parkour(block)
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		false, ignores, DebugType, HitResults[(int32)EParkourArrowType::Land], true, color, FLinearColor::White);
}




