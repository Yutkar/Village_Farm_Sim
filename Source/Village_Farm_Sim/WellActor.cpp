#include "WellActor.h"
#include "WateringCanActor.h"

AWellActor::AWellActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetSimulatePhysics(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    Tags.Add(TEXT("Well"));
}

bool AWellActor::FillWateringCan(AWateringCanActor* WateringCan)
{
    if (!WateringCan)
    {
        return false;
    }

    WateringCan->FillFull();
    return true;
}