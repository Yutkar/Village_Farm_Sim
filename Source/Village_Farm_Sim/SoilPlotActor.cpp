#include "SoilPlotActor.h"
#include "CarrotPlantActor.h"

ASoilPlotActor::ASoilPlotActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SoilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoilMesh"));
    RootComponent = SoilMesh;

    SoilMesh->SetSimulatePhysics(false);
    SoilMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    Tags.Add(TEXT("SoilPlot"));
}

void ASoilPlotActor::BeginPlay()
{
    Super::BeginPlay();

    UpdateSoilVisual();
}

void ASoilPlotActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    UpdateSoilVisual();
}

bool ASoilPlotActor::CanDig() const
{
    return SoilState == ESoilState::Normal;
}

bool ASoilPlotActor::AddDigProgress(float Amount)
{
    if (!CanDig())
    {
        return false;
    }

    DigProgress += Amount;

    if (DigProgress >= RequiredDigProgress)
    {
        DigProgress = RequiredDigProgress;
        SoilState = ESoilState::Loosened;
        UpdateSoilVisual();

        return true;
    }

    return false;
}

bool ASoilPlotActor::CanPlantCarrot() const
{
    return SoilState == ESoilState::Loosened && CurrentPlant == nullptr && CarrotPlantClass != nullptr;
}

bool ASoilPlotActor::PlantCarrot()
{
    if (!CanPlantCarrot())
    {
        return false;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        return false;
    }

    const FVector SpawnLocation = GetActorLocation() + PlantSpawnOffset;
    const FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    ACarrotPlantActor* SpawnedPlant = World->SpawnActor<ACarrotPlantActor>(
        CarrotPlantClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (!SpawnedPlant)
    {
        return false;
    }

    CurrentPlant = SpawnedPlant;
    SpawnedPlant->OwnerSoil = this;

    SoilState = ESoilState::Growing;
    UpdateSoilVisual();

    return true;
}

bool ASoilPlotActor::WaterSoil(float Amount)
{
    if (!CurrentPlant || Amount <= 0.0f)
    {
        return false;
    }

    CurrentPlant->WaterPlant(Amount);
    return true;
}

void ASoilPlotActor::ClearPlant()
{
    CurrentPlant = nullptr;
    SoilState = ESoilState::EmptyAfterHarvest;
    UpdateSoilVisual();
}

void ASoilPlotActor::UpdateSoilVisual()
{
    if (!SoilMesh)
    {
        return;
    }

    switch (SoilState)
    {
    case ESoilState::Normal:
        if (NormalMaterial)
        {
            SoilMesh->SetMaterial(0, NormalMaterial);
        }
        break;

    case ESoilState::Loosened:
        if (LoosenedMaterial)
        {
            SoilMesh->SetMaterial(0, LoosenedMaterial);
        }
        break;

    case ESoilState::Growing:
    case ESoilState::EmptyAfterHarvest:
        if (GrowingMaterial)
        {
            SoilMesh->SetMaterial(0, GrowingMaterial);
        }
        break;

    default:
        break;
    }
}