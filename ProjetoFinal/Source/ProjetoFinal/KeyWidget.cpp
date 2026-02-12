#include "KeyWidget.h"
#include "KeyInventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

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
    
    // Also keep trying every second until we get the inventory
    GetWorld()->GetTimerManager().SetTimer(RefreshTimerHandle, this, 
        &UKeyWidget::RefreshInventoryBinding, 0.5f, true);
}

void UKeyWidget::NativeDestruct()
{
    GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    Super::NativeDestruct();
}

void UKeyWidget::RefreshInventoryBinding()
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) 
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No player controller yet"));
        return;
    }

    ACharacter* Character = PC->GetCharacter();
    if (!Character) 
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No character yet"));
        return;
    }

    UKeyInventoryComponent* Inventory = Character->FindComponentByClass<UKeyInventoryComponent>();
    if (!Inventory) 
    {
        UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: No inventory component yet"));
        return;
    }

    // Found inventory! Clear timer and bind
    GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    
    // Remove old binding if exists
    Inventory->OnKeyCountChanged.RemoveDynamic(this, &UKeyWidget::OnKeyCountChanged);
    // Add new binding
    Inventory->OnKeyCountChanged.AddDynamic(this, &UKeyWidget::OnKeyCountChanged);
    
    UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: Successfully bound to inventory"));
    
    // Force an immediate update
    int32 CurrentCount = Inventory->GetKeyCount(KeyID);
    OnKeyCountChanged(KeyID, CurrentCount);
}

void UKeyWidget::OnKeyCountChanged(FName UpdatedKeyID, int32 NewCount)
{
    if (UpdatedKeyID != KeyID) return;

    UE_LOG(LogTemp, Warning, TEXT("KEYWIDGET: Updating %s to %d"), *KeyID.ToString(), NewCount);

    if (KeyIcon && KeyIconTexture)
    {
        KeyIcon->SetBrushFromTexture(KeyIconTexture);
        KeyIcon->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (KeyCountText)
    {
        KeyCountText->SetText(FText::AsNumber(NewCount));
        KeyCountText->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}