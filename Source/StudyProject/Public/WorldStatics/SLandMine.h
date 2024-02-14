// SLandMine.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLandMine.generated.h"

UCLASS()
class STUDYPROJECT_API ASLandMine : public AActor
{
    GENERATED_BODY()

    public:
    ASLandMine();

    UFUNCTION()
    void OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(NetMulticast, Unreliable)
    void SpawnEffect_NetMulticast();

    UFUNCTION()
    void OnRep_IsExploded();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
   
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", Meta = (AllowPrivateAccess))
    TObjectPtr<class UBoxComponent> BodyBoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", Meta = (AllowPrivateAccess))
    TObjectPtr<class UStaticMeshComponent> BodyStaticMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
    TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

    UPROPERTY(ReplicatedUsing = OnRep_IsExploded, VisibleAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
    uint8 bIsExploded : 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASLandMine", meta = (AllowPrivateAccess))
    TObjectPtr<class UMaterial> ExplodedMaterial;


};