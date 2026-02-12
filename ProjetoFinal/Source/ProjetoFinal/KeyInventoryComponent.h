#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeyCountChanged, FName, KeyID, int32, NewCount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJETOFINAL_API UKeyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKeyInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Key")
	void AddKey(FName KeyID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Key")
	bool HasKey(FName KeyID, int32 RequiredAmount = 1);

	UFUNCTION(BlueprintCallable, Category = "Key")
	bool ConsumeKey(FName KeyID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Key")
	int32 GetKeyCount(FName KeyID);

	UPROPERTY(BlueprintAssignable, Category = "Key")
	FOnKeyCountChanged OnKeyCountChanged;

private:
	UPROPERTY()
	TMap<FName, int32> Keys;
};