#include "ARespawnPoint.h"

ARespawnPoint::ARespawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;
    
	if (IconTexture)
	{
		Billboard->SetSprite(IconTexture);
	}
	Billboard->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    
	PointName = FName(TEXT("Respawn Point"));
}