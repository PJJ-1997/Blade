#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include  "NiagaraDataInterfaceExport.h"
#include "CAura.generated.h"


// INiagaraParticleCallbackHandler : �ý��ۿ��� ������ ���� �����Ѵ�
// typelib
UCLASS()
class U06_BATTLE_API ACAura : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:	
	ACAura();

protected:
	virtual void BeginPlay() override;


private:
	UFUNCTION()
		void OnSystemFinished(class UNiagaraComponent* PSyetem);


public:
	//INiagaraParticleCallbackHandler �� �Լ�����
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Niagara")
	//void ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem);
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem) override;


private:
    UFUNCTION()
        void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex);





private: // Hit���� ������
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        FHitData HitData;
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        float DamageInterval = 0.1f;

private:
    UPROPERTY(VisibleAnywhere)
        class USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
        class UBoxComponent* Box;
    UPROPERTY(VisibleAnywhere)
        class UNiagaraComponent* Niagara;



private:
    TArray<class ACharacter*> Hit;
    FTimerHandle TimerHandle;

};
