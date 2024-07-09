#include "Weapons/AddOns/CAura.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"


ACAura::ACAura()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'");
	Niagara->SetAsset(niagara);

}

void ACAura::BeginPlay()
{
	Super::BeginPlay();
	Niagara->SetNiagaraVariableObject("Mesh_Scale", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACAura::OnSystemFinished);


    // Hit
    Box->OnComponentBeginOverlap.AddDynamic(this,&ACAura::OnComponentBeginOverlap);
    Box->OnComponentEndOverlap.AddDynamic(this,&ACAura::OnComponentEndOverlap);

    // 시간경과후 time로 진입
    // 람다 함수
    // [캡쳐]   :  =,&(CallByReference) 참조,복사하는 방식
    // ()       : 매개변수
    // ->Return Type
    // {        : 몸통
    // }

   
 //   CLog::Print("....................");
 //   CLog::Print(Hit.Num());

   
    FTimerDelegate timeDelegate = FTimerDelegate::CreateLambda([&]()->void
        {

            for (int32 i = Hit.Num() - 1; i >= 0; i--)
                HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hit[i]);

    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, timeDelegate, DamageInterval, true, 0);
	
}
void ACAura::OnSystemFinished(class UNiagaraComponent* PSyetem)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	Destroy();

}


void ACAura::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem)
{
//	CLog::Log(Data[0].Position);
//	CLog::Log(Data[0].Size);

 //   Niagara->Deactivate();
 //   return;
	Box->SetRelativeScale3D(Data[0].Position);

	FVector location = Box->GetScaledBoxExtent();
	location.Y = 0;    // 2D이기 때문에 Culling해주기 위해
	Box->SetRelativeLocation(location);

}

void ACAura::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    CheckTrue(GetOwner() == OtherActor);

    ACharacter* character = Cast<ACharacter>(OtherActor);
    if (!!character)
        Hit.AddUnique(character);


}
void ACAura::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
    CheckTrue(GetOwner() == OtherActor);

    ACharacter* character = Cast<ACharacter>(OtherActor);
    if (!!character)
        Hit.Remove(character);
}