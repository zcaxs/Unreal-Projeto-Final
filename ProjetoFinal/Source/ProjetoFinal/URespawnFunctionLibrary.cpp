#include "URespawnFunctionLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void URespawnFunctionLibrary::RespawnAtNearestRespawnPoint(UObject* WorldContextObject, AActor* DeadCharacter)
{
    if (!WorldContextObject || !DeadCharacter) 
    {
        UE_LOG(LogTemp, Error, TEXT("RESPAWN FAILED: Invalid WorldContext or DeadCharacter"));
        return;
    }
    
    ACharacter* Character = Cast<ACharacter>(DeadCharacter);
    if (!Character) 
    {
        UE_LOG(LogTemp, Error, TEXT("RESPAWN FAILED: DeadCharacter is not a Character"));
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) 
    {
        UE_LOG(LogTemp, Error, TEXT("RESPAWN FAILED: Invalid World"));
        return;
    }

    // Get ALL actors
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);
    
    // Filter to only those that are ARespawnPoint or children
    TArray<AActor*> RespawnPoints;
    for (AActor* Actor : AllActors)
    {
        // DEBUG: Check every actor's class hierarchy
        UE_LOG(LogTemp, Warning, TEXT("  - Checking actor: %s, Class: %s, Parent: %s"), 
            *Actor->GetName(),
            *Actor->GetClass()->GetName(),
            *Actor->GetClass()->GetSuperClass()->GetName());

        if (Cast<ARespawnPoint>(Actor))
       // if (Actor->IsA(ARespawnPoint::StaticClass()))
        {
            RespawnPoints.Add(Actor);
            UE_LOG(LogTemp, Warning, TEXT("  - ✓ MATCH: %s is a RespawnPoint"), *Actor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("  - ✗ NOT MATCH: %s is NOT a RespawnPoint"), *Actor->GetName());
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("RESPAWN: Found %d RespawnPoint actors in level"), RespawnPoints.Num());
    
    for (AActor* Point : RespawnPoints)
    {
        UE_LOG(LogTemp, Log, TEXT("  - Respawn Point: %s at %s"), 
            *Point->GetName(), 
            *Point->GetActorLocation().ToString());
    }
    
    if (RespawnPoints.Num() == 0) 
    {
        UE_LOG(LogTemp, Error, TEXT("RESPAWN FAILED: No respawn points found in level!"));
        return;
    }

    AActor* NearestPoint = nullptr;
    float MinDistance = FLT_MAX;
    FVector DeathLocation = Character->GetActorLocation();

    for (AActor* Point : RespawnPoints)
    {
        float Distance = FVector::Dist(DeathLocation, Point->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            NearestPoint = Point;
        }
    }

    if (NearestPoint)
    {
        Character->TeleportTo(NearestPoint->GetActorLocation(), NearestPoint->GetActorRotation());
        UE_LOG(LogTemp, Warning, TEXT("RESPAWN SUCCESS: Teleported to %s (distance: %f)"), 
            *NearestPoint->GetName(), MinDistance);
    }
}