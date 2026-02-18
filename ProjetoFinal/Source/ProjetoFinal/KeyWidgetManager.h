#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyWidget.h"
#include "KeyWidgetManager.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName="My Widget Manager"))
class PROJETOFINAL_API AKeyWidgetManager : public AActor
{
	GENERATED_BODY()
    
public:
	AKeyWidgetManager();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Key Widget")
	TSubclassOf<UKeyWidget> KeyWidgetClass;

	UPROPERTY(EditAnywhere, Category="Key Widget")
	FName KeyID;

	UPROPERTY(EditAnywhere, Category="Key Widget")
	UTexture2D* KeyIconTexture;

	UFUNCTION()
	void OnPlayerControllerReady();

private:
	FTimerHandle WidgetSpawnTimerHandle;

	// Keep a reference so the widget isn't garbage collected
	UPROPERTY()
	UKeyWidget* CreatedWidget = nullptr;

	// Make actor visible/selectable in editor
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USceneComponent* RootScene = nullptr;
};