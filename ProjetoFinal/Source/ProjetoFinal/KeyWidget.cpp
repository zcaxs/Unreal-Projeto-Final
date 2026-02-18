#include "KeyWidget.h"
#include "KeyInventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UKeyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set initial visibility
    if (KeyIcon)
    {
        KeyIcon->SetVisibility(ESlateVisibility::Hidden);
    }
    if (KeyCountText)
    {
        KeyCountText->SetVisibility(ESlateVisibility::Hidden);
    }

    // Try to bind immediately
    RefreshInventoryBinding();
    
    // Also keep trying every half-second until we get the inventory
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(RefreshTimerHandle, this, 
            &UKeyWidget::RefreshInventoryBinding, 0.5f, true);
    }
}

void UKeyWidget::NativeDestruct()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    }
    Super::NativeDestruct();
}

void UKeyWidget::RefreshInventoryBinding()
{
    // Guard world
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No world yet"));
        return;
    }

    // Try to get the owning pawn first (works for Character or Pawn)
    APawn* Pawn = nullptr;
    if (APlayerController* PC = GetOwningPlayer())
    {
        Pawn = PC->GetPawn();
    }

    // Fallback to player pawn index 0
    if (!Pawn)
    {
        Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
    }

    if (!Pawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No pawn/player yet"));
        return;
    }

    UKeyInventoryComponent* Inventory = Pawn->FindComponentByClass<UKeyInventoryComponent>();
    if (!Inventory)
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No inventory component found on %s"), *Pawn->GetName());
        return;
    }

    // Found inventory! Clear timer and bind
    World->GetTimerManager().ClearTimer(RefreshTimerHandle);

    // Remove old binding if exists then add new binding
    Inventory->OnKeyCountChanged.RemoveDynamic(this, &UKeyWidget::OnKeyCountChanged);
    Inventory->OnKeyCountChanged.AddDynamic(this, &UKeyWidget::OnKeyCountChanged);
    
    UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: Successfully bound to inventory on %s for KeyID=%s"), 
        *Pawn->GetName(), *KeyID.ToString());

    // If we have a key icon texture configured, set it now
    if (KeyIcon && KeyIconTexture)
    {
        KeyIcon->SetBrushFromTexture(KeyIconTexture);
    }

    // Force an immediate update using the current count (this ensures we don't miss keys added before bind)
    int32 CurrentCount = Inventory->GetKeyCount(KeyID);
    OnKeyCountChanged(KeyID, CurrentCount);
}

void UKeyWidget::OnKeyCountChanged(FName UpdatedKeyID, int32 NewCount)
{
    if (UpdatedKeyID != KeyID) return;

    UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: Updating KeyID=%s to %d"), *KeyID.ToString(), NewCount);

    if (KeyIcon)
    {
        // Only set brush if a texture was provided
        if (KeyIconTexture)
        {
            KeyIcon->SetBrushFromTexture(KeyIconTexture);
        }
        KeyIcon->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (KeyCountText)
    {
        KeyCountText->SetText(FText::AsNumber(NewCount));
        KeyCountText->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}