#include "DynamicSoilManagerActor.h"
#include "SoilPlotActor.h"
#include "Engine/World.h"

ADynamicSoilManagerActor::ADynamicSoilManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

FIntPoint ADynamicSoilManagerActor::WorldLocationToCell(const FVector& WorldLocation) const
{
    const int32 CellX = FMath::RoundToInt(WorldLocation.X / CellSize);
    const int32 CellY = FMath::RoundToInt(WorldLocation.Y / CellSize);

    return FIntPoint(CellX, CellY);
}

FVector ADynamicSoilManagerActor::CellToWorldLocation(const FIntPoint& Cell, float OriginalZ) const
{
    return FVector(
        static_cast<float>(Cell.X) * CellSize,
        static_cast<float>(Cell.Y) * CellSize,
        OriginalZ + SoilSpawnZOffset
    );
}

ASoilPlotActor* ADynamicSoilManagerActor::GetOrCreateSoilPlotAtLocation(const FVector& WorldLocation)
{
    if (!SoilPlotClass)
    {
        return nullptr;
    }

    const FIntPoint Cell = WorldLocationToCell(WorldLocation);

    if (ASoilPlotActor** ExistingPlot = SpawnedSoilPlots.Find(Cell))
    {
        return *ExistingPlot;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        return nullptr;
    }

    const FVector SpawnLocation = CellToWorldLocation(Cell, WorldLocation.Z);
    const FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    ASoilPlotActor* NewPlot = World->SpawnActor<ASoilPlotActor>(
        SoilPlotClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (!NewPlot)
    {
        return nullptr;
    }

    SpawnedSoilPlots.Add(Cell, NewPlot);

    return NewPlot;
}

bool ADynamicSoilManagerActor::DigAtWorldLocation(const FVector& WorldLocation, float DeltaTime)
{
    if (DeltaTime <= 0.0f)
    {
        return false;
    }

    ASoilPlotActor* SoilPlot = GetOrCreateSoilPlotAtLocation(WorldLocation);

    if (!SoilPlot)
    {
        return false;
    }

    const float DigAmount = DigAmountPerSecond * DeltaTime;

    SoilPlot->AddDigProgress(DigAmount);

    return true;
}