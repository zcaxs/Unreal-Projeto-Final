#include "KeyWidgetManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AKeyWidgetManager::AKeyWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	KeyID = FName(TEXT("Default"));
}

void AKeyWidgetManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Wait a bit for player to spawn
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AKeyWidgetManager::OnPlayerControllerReady, 0.2f, false);
}

void AKeyWidgetManager::OnPlayerControllerReady()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC || !KeyWidgetClass) return;

    UKeyWidget* Widget = CreateWidget<UKeyWidget>(PC, KeyWidgetClass);
    if (Widget)
    {
        Widget->KeyID = KeyID;
        Widget->KeyIconTexture = KeyIconTexture;
        Widget->AddToViewport();
        UE_LOG(LogTemp, Warning, TEXT("KEYMANAGER: Widget created, will auto-bind when inventory exists"));
    }
}