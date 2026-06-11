#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "SoilPlotActor.generated.h"

class ACarrotPlantActor;

UENUM(BlueprintType)
enum class ESoilState : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Loosened UMETA(DisplayName = "Loosened"),
    Growing UMETA(DisplayName = "Growing"),
    EmptyAfterHarvest UMETA(DisplayName = "EmptyAfterHarvest")
};

UCLASS()
class VILLAGE_FARM_SIM_API ASoilPlotActor : public AActor
{
    GENERATED_BODY()

public:
    ASoilPlotActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SoilMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
    ESoilState SoilState = ESoilState::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
    float DigProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
    float RequiredDigProgress = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
    float DigPowerPerSecond = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
    TSubclassOf<ACarrotPlantActor> CarrotPlantClass;

    UPROPERTY(BlueprintReadOnly, Category = "Plant")
    ACarrotPlantActor* CurrentPlant = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
    FVector PlantSpawnOffset = FVector(0.0f, 0.0f, 5.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* NormalMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* LoosenedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UMaterialInterface* GrowingMaterial;

    UFUNCTION(BlueprintCallable, Category = "Soil")
    bool CanDig() const;

    UFUNCTION(BlueprintCallable, Category = "Soil")
    bool AddDigProgress(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Soil")
    bool CanPlantCarrot() const;

    UFUNCTION(BlueprintCallable, Category = "Soil")
    bool PlantCarrot();

    UFUNCTION(BlueprintCallable, Category = "Soil")
    bool WaterSoil(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Soil")
    void ClearPlant();

    UFUNCTION(BlueprintCallable, Category = "Soil")
    void UpdateSoilVisual();
};