#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ButtonObject.generated.h"

UCLASS()
class PROJETOFINAL_API AButtonObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AButtonObject();

	UFUNCTION(BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ObjectsToReveal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RequiredJumpForce = -500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOneTimeUse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* ActiveMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* DeactiveMaterial = nullptr;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ButtonMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox = nullptr;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	FVector OriginalLocation;
};