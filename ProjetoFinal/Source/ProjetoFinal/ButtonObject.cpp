#include "ButtonObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstance.h"

AButtonObject::AButtonObject()
{
	PrimaryActorTick.bCanEverTick = false;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	RootComponent = ButtonMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));

	bIsActive = false;
}

void AButtonObject::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalLocation = GetActorLocation();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AButtonObject::OnOverlapBegin);

	if (DeactiveMaterial && ButtonMesh)
	{
		ButtonMesh->SetMaterial(1, DeactiveMaterial);
	}
}

void AButtonObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActive && bOneTimeUse) return;
	if (!OtherActor) return;

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	if (!Movement) return;

	float VelocityZ = Movement->Velocity.Z;

	if (VelocityZ <= RequiredJumpForce)
	{
		Activate();
	}
}

void AButtonObject::Activate()
{
	if (bIsActive) return;
	
	bIsActive = true;

	if (ActiveMaterial && ButtonMesh)
	{
		ButtonMesh->SetMaterial(1, ActiveMaterial);
	}

	for (AActor* Object : ObjectsToReveal)
	{
		if (Object)
		{
			Object->SetActorHiddenInGame(false);
			Object->SetActorEnableCollision(true);
			
			TArray<UStaticMeshComponent*> Meshes;
			Object->GetComponents<UStaticMeshComponent>(Meshes);
			
			for (UStaticMeshComponent* Mesh : Meshes)
			{
				Mesh->SetVisibility(true);
				Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= 10.0f;
	SetActorLocation(NewLocation);
}

void AButtonObject::Deactivate()
{
	if (!bIsActive) return;
	
	bIsActive = false;

	if (DeactiveMaterial && ButtonMesh)
	{
		ButtonMesh->SetMaterial(1, DeactiveMaterial);
	}

	for (AActor* Object : ObjectsToReveal)
	{
		if (Object)
		{
			Object->SetActorHiddenInGame(true);
			Object->SetActorEnableCollision(false);
			
			TArray<UStaticMeshComponent*> Meshes;
			Object->GetComponents<UStaticMeshComponent>(Meshes);
			
			for (UStaticMeshComponent* Mesh : Meshes)
			{
				Mesh->SetVisibility(false);
				Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}

	SetActorLocation(OriginalLocation);
}