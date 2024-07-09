#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include "CActor_RangeSword.generated.h"

UCLASS()
class U06_BATTLE_API ACActor_RangeSword : public AActor
{
	GENERATED_BODY()

public:
	ACActor_RangeSword();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
		float Speed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
		float RotateSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
		FName EnemyTag = "Player";

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float DamageInterval = 0.1f;

	void SetTarget(AActor* NewTarget);
private:
	AActor* Target;
	void FindTarget();


	float HomingDuration = 3.0f;  // ���� �ð� (��)
	float HomingTimeElapsed = 0.0f;  // ��� �ð�

	FTimerHandle  TimerHandle; // SendMessage()�� Timer�� ���ε��� �ڵ�
	TArray<ACharacter*> Hit;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;


	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void SendMessage();

};