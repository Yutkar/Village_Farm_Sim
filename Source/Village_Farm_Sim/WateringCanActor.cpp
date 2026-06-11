#include "WateringCanActor.h"

AWateringCanActor::AWateringCanActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetSimulatePhysics(true);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionObjectType(ECC_PhysicsBody);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);

    Tags.Add(TEXT("Grabbable"));
    Tags.Add(TEXT("WateringCan"));
}

void AWateringCanActor::FillFull()
{
    CurrentWater = MaxWater;
}

float AWateringCanActor::UseWater(float Amount)
{
    if (Amount <= 0.0f || CurrentWater <= 0.0f)
    {
        return 0.0f;
    }

    const float UsedAmount = FMath::Min(CurrentWater, Amount);
    CurrentWater -= UsedAmount;

    return UsedAmount;
}

bool AWateringCanActor::HasWater() const
{
    return CurrentWater > 0.0f;
}