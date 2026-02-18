#include "KeyWidgetManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

AKeyWidgetManager::AKeyWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	KeyID = FName(TEXT("Default"));
	CreatedWidget = nullptr;

	// Create a simple root so actor is visible and placeable in the editor
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
}

void AKeyWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	// Start polling for the player controller so we create the widget reliably.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(WidgetSpawnTimerHandle, this,
			&AKeyWidgetManager::OnPlayerControllerReady, 0.2f, true);
	}
}

void AKeyWidgetManager::OnPlayerControllerReady()
{
	// If already created, nothing to do
	if (CreatedWidget)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(WidgetSpawnTimerHandle);
		}
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("KEYMANAGER: No world yet"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("KEYMANAGER: PlayerController not ready"));
		return;
	}

	if (!KeyWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("KEYMANAGER: KeyWidgetClass is not set on KeyWidgetManager"));
		World->GetTimerManager().ClearTimer(WidgetSpawnTimerHandle);
		return;
	}

	// Create the widget owned by the local player controller right away (no pawn required)
	UKeyWidget* Widget = CreateWidget<UKeyWidget>(PC, KeyWidgetClass);
	if (Widget)
	{
		Widget->KeyID = KeyID;
		Widget->KeyIconTexture = KeyIconTexture;
		Widget->AddToViewport();

		// Hold a reference so GC doesn't remove it
		CreatedWidget = Widget;

		UE_LOG(LogTemp, Warning, TEXT("KEYMANAGER: Widget created and added to viewport for player %s"), *PC->GetName());

		// Stop retry timer
		World->GetTimerManager().ClearTimer(WidgetSpawnTimerHandle);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("KEYMANAGER: Failed to create widget instance"));
	}
}