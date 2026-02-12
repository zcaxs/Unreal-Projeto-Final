#include "KeyInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UKeyInventoryComponent::UKeyInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UKeyInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UKeyInventoryComponent::AddKey(FName KeyID, int32 Amount)
{
    if (Amount <= 0) return;

    int32& CurrentCount = Keys.FindOrAdd(KeyID);
    CurrentCount += Amount;
    
    UE_LOG(LogTemp, Warning, TEXT("INVENTORY: Key %s count is now %d"), *KeyID.ToString(), CurrentCount);
    UE_LOG(LogTemp, Warning, TEXT("INVENTORY: Delegate has %d bound listeners"), OnKeyCountChanged.GetAllObjects().Num());
    
    OnKeyCountChanged.Broadcast(KeyID, CurrentCount);
}

bool UKeyInventoryComponent::HasKey(FName KeyID, int32 RequiredAmount)
{
    if (const int32* Count = Keys.Find(KeyID))
    {
        return *Count >= RequiredAmount;
    }
    return false;
}

bool UKeyInventoryComponent::ConsumeKey(FName KeyID, int32 Amount)
{
    if (Amount <= 0) return true;

    int32* Count = Keys.Find(KeyID);
    if (!Count || *Count < Amount) return false;

    *Count -= Amount;
    
    if (*Count <= 0)
    {
        Keys.Remove(KeyID);
        OnKeyCountChanged.Broadcast(KeyID, 0);
    }
    else
    {
        OnKeyCountChanged.Broadcast(KeyID, *Count);
    }
    return true;
}

int32 UKeyInventoryComponent::GetKeyCount(FName KeyID)
{
    if (const int32* Count = Keys.Find(KeyID))
    {
        return *Count;
    }
    return 0;
}