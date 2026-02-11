#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "ARespawnPoint.generated.h"

UCLASS()
class PROJETOFINAL_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
    
public:
	ARespawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* Billboard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PointName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* IconTexture = nullptr;
};