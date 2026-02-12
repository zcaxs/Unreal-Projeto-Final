#include "KeyActor.h"
#include "KeyInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AKeyActor::AKeyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyID = FName(TEXT("Default"));
	KeyValue = 1;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AKeyActor::OnOverlapBegin);
}

void AKeyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
    bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character) return;

    UKeyInventoryComponent* Inventory = Character->FindComponentByClass<UKeyInventoryComponent>();
    
    if (!Inventory)
    {
        Inventory = NewObject<UKeyInventoryComponent>(Character);
        Inventory->RegisterComponent();
        Character->AddInstanceComponent(Inventory);
        UE_LOG(LogTemp, Warning, TEXT("KEY: Created new inventory component for %s"), *Character->GetName());
    }

    UE_LOG(LogTemp, Warning, TEXT("KEY: Adding %d of %s to inventory"), KeyValue, *KeyID.ToString());
    Inventory->AddKey(KeyID, KeyValue);
    
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }
    
    Destroy();
}