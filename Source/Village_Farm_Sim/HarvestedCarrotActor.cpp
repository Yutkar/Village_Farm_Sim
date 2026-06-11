#include "HarvestedCarrotActor.h"

AHarvestedCarrotActor::AHarvestedCarrotActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetSimulatePhysics(true);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionObjectType(ECC_PhysicsBody);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);

    Tags.Add(TEXT("Grabbable"));
    Tags.Add(TEXT("HarvestedCarrot"));
}