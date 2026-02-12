#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GateActor.generated.h"

UCLASS()
class PROJETOFINAL_API AGateActor : public AActor
{
    GENERATED_BODY()
    
public:
    AGateActor();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerZone;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* GateMesh;

    UPROPERTY(EditAnywhere)
    FName RequiredKeyID;

    UPROPERTY(EditAnywhere)
    int32 RequiredKeyCount;

    UPROPERTY(EditAnywhere)
    float OpenHeight;

    UPROPERTY(EditAnywhere)
    float OpenSpeed;

    UPROPERTY(EditAnywhere)
    USoundBase* OpenSound;

    UPROPERTY(EditAnywhere)
    USoundBase* DeniedSound;

    UPROPERTY(BlueprintReadOnly)
    bool bIsOpen;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
        bool bFromSweep, const FHitResult& SweepResult);

    void Interact(AActor* Interactor);

private:
    FVector ClosedLocation;
    FVector OpenLocation;
};