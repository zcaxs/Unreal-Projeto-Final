#include "GateActor.h"
#include "KeyInventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGateActor::AGateActor()
{
    PrimaryActorTick.bCanEverTick = true;

    TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    RootComponent = TriggerZone;
    TriggerZone->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
    TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
    GateMesh->SetupAttachment(RootComponent);

    RequiredKeyID = FName(TEXT("Default"));
    RequiredKeyCount = 1;
    OpenHeight = 300.0f;
    OpenSpeed = 5.0f;
    bIsOpen = false;

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AGateActor::OnOverlapBegin);
}

void AGateActor::BeginPlay()
{
    Super::BeginPlay();
    ClosedLocation = GateMesh->GetRelativeLocation();
    OpenLocation = ClosedLocation + FVector(0.0f, 0.0f, OpenHeight);
}

void AGateActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector TargetLocation = bIsOpen ? OpenLocation : ClosedLocation;
    FVector CurrentLocation = GateMesh->GetRelativeLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, OpenSpeed);
    GateMesh->SetRelativeLocation(NewLocation);
}

void AGateActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
    bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character || !Character->IsLocallyControlled()) return;

    Interact(Character);
}

void AGateActor::Interact(AActor* Interactor)
{
    ACharacter* Character = Cast<ACharacter>(Interactor);
    if (!Character) return;

    UKeyInventoryComponent* Inventory = Character->FindComponentByClass<UKeyInventoryComponent>();
    
    // Auto-add component if not exists (should exist from key pickup)
    if (!Inventory)
    {
        Inventory = NewObject<UKeyInventoryComponent>(Character);
        Inventory->RegisterComponent();
        Character->AddInstanceComponent(Inventory);
    }

    if (Inventory->HasKey(RequiredKeyID, RequiredKeyCount))
    {
        Inventory->ConsumeKey(RequiredKeyID, RequiredKeyCount);
        bIsOpen = true;
        
        if (OpenSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
        }
    }
    else if (DeniedSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeniedSound, GetActorLocation());
    }
}