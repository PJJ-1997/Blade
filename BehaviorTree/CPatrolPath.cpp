// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	this->bRunConstructionScriptOnDrag = false;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Root);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30)); // Z������ 30, �����ְ�
	Spline->bHiddenInGame = false;

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;

}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPatrolPath::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	Text->Text = FText::FromString(GetActorLabel());
#endif

	Spline->SetClosedLoop(bLoop);

}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}
		// ���� �ε����� 0�ΰ��  :     3->2->1->0  ---> 0->3
		if (Spline->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}
		// ���� �ε����� 0�ΰ��  :     3->2->1->0  ---> 0->1
		{
			Index = 1;
			bReverse = false;
			return;
		}
	}
	// ���� �ε����� count���� �������  :     0->1->2->3  --->  0->1
	if (Index < (count - 1))
	{
		Index++;
		return;
	}
	// ���� �ε����� 3�ΰ��  :       0->1->2->3  --->  3->0
	if (Spline->IsClosedLoop())
	{
		Index = 0;
		return;

	}
	// ���� �ε����� 3�ΰ��  :       0->1->2->3  --->  3->2
	{
		Index = count - 2;
		bReverse = true;
	}
}


