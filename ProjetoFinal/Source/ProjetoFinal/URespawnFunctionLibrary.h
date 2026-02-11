#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARespawnPoint.h"
#include "URespawnFunctionLibrary.generated.h"

UCLASS()
class PROJETOFINAL_API URespawnFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Respawn", meta = (WorldContext = "WorldContextObject"))
	static void RespawnAtNearestRespawnPoint(UObject* WorldContextObject, AActor* DeadCharacter);
};