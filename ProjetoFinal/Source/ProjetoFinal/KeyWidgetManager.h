#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyWidget.h"
#include "KeyWidgetManager.generated.h"

UCLASS()
class PROJETOFINAL_API AKeyWidgetManager : public AActor
{
	GENERATED_BODY()
    
public:
	AKeyWidgetManager();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKeyWidget> KeyWidgetClass;

	UPROPERTY(EditAnywhere)
	FName KeyID;

	UPROPERTY(EditAnywhere)
	UTexture2D* KeyIconTexture;

	UFUNCTION()
	void OnPlayerControllerReady();
};