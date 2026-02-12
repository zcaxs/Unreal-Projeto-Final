#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KeyActor.generated.h"

UCLASS()
class PROJETOFINAL_API AKeyActor : public AActor
{
	GENERATED_BODY()
    
public:
	AKeyActor();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	FName KeyID;

	UPROPERTY(EditAnywhere)
	int32 KeyValue;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);
};