#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"

ACAttachment::ACAttachment()
{
    CHelpers::CreateComponent(this, &Root, "Root");

}

void ACAttachment::BeginPlay()
{

    // 주)  Super::BeginPlay()이전 함수가 처리되러 있어야 한다
    // ACAttachment
    //    - BP_CAttament_Sword 
    //          Root
    //          |--- SkeletalMesh
    //              |-- Capsule

    OwnerCharacter = Cast<ACharacter>(GetOwner());

    Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
    State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);


    TArray<USceneComponent*> childern;



    //
    // USceneComponent
    //   |--  UShapeCompont
    //      |--  UBoxComponent
    //      |--  UCapsuleComponent
    //      |--  USphereComponent
    //

    Root->GetChildrenComponents(true, childern);  // collider(sword) 1,  collider(fist) 4

    for (USceneComponent* child : childern)
    {
        UShapeComponent* shape = Cast<UShapeComponent>(child);
        if (!!shape)  // sword : UCapsuleComponent, fist : USphereComponent
           {
            shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
            shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
            // Collision on/off를 콘트롤하기 위해 변수 저장
            Collisions.Add(shape);
           }
    }

    // Collion Off
    OffCollisions();

	Super::BeginPlay(); // Actor가 BeginPlay
	
}

void ACAttachment::OnCollisions()
{

    if (OnAttachmentBeginCollision.IsBound())
        OnAttachmentBeginCollision.Broadcast();

    for (UShapeComponent* shape : Collisions)
    {
        shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

}

void ACAttachment::OffCollisions()
{
    if (OnAttachmentEndCollision.IsBound())
        OnAttachmentEndCollision.Broadcast();

    for (UShapeComponent* shape : Collisions)
    {
        shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    CheckTrue(OwnerCharacter == OtherActor);
    CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

    if (OnAttachmentBeginOverlap.IsBound())
        OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));


}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    CheckTrue(OwnerCharacter == OtherActor);
    CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());


    if (OnAttachmentEndOverlap.IsBound())
        OnAttachmentEndOverlap.Broadcast(OwnerCharacter,  Cast<ACharacter>(OtherActor));
}

void ACAttachment::AttachTo(FName InSocketName)
{
    AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void  ACAttachment::AttachToCollision(FName InSocketName)
{
    for (UShapeComponent* collision : Collisions)
    {
        if (collision->GetName() == InSocketName.ToString())
        {
            collision->AttachToComponent(OwnerCharacter->GetMesh(),
                FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
        }
    }
}