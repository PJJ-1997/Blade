// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

class ACPlayer;

UCLASS()
class U06_BATTLE_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInterfaceTestActor();

protected:
	UPROPERTY(EditAnywhere, Category = "Test Actor")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
		FInteractableData InstanceInteractableData;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus()		   override;
	virtual void EndFocus()			   override;
	virtual void BeginInteract()	   override;
	virtual void EndInteract()		   override;
	virtual void Interact(ACPlayer* PlayerCharacter) override;
};
