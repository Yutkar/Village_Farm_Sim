#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicSoilManagerActor.generated.h"

class ASoilPlotActor;

UCLASS()
class VILLAGE_FARM_SIM_API ADynamicSoilManagerActor : public AActor
{
    GENERATED_BODY()

public:
    ADynamicSoilManagerActor();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Soil")
    TSubclassOf<ASoilPlotActor> SoilPlotClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Soil")
    float CellSize = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Soil")
    float DigAmountPerSecond = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Soil")
    float MaxDigDistanceFromPlayer = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Soil")
    float SoilSpawnZOffset = 2.0f;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Soil")
    bool DigAtWorldLocation(const FVector& WorldLocation, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Soil")
    ASoilPlotActor* GetOrCreateSoilPlotAtLocation(const FVector& WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "Dynamic Soil")
    FIntPoint WorldLocationToCell(const FVector& WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category = "Dynamic Soil")
    FVector CellToWorldLocation(const FIntPoint& Cell, float OriginalZ) const;

private:
    TMap<FIntPoint, ASoilPlotActor*> SpawnedSoilPlots;
};