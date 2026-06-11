#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CarrotPlantActor.generated.h"

class ASoilPlotActor;
class AHarvestedCarrotActor;

UCLASS()
class VILLAGE_FARM_SIM_API ACarrotPlantActor : public AActor
{
    GENERATED_BODY()

public:
    ACarrotPlantActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* LeavesMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* RootMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* RottenMesh;

    UPROPERTY(BlueprintReadWrite, Category = "Plant")
    ASoilPlotActor* OwnerSoil = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Harvest")
    TSubclassOf<AHarvestedCarrotActor> HarvestedCarrotClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Growth")
    int32 GrowthStage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Growth")
    int32 MaxGrowthStage = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Growth")
    float GrowthProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Growth")
    float GrowthPerTick = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|State")
    bool bIsReadyToHarvest = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|State")
    bool bIsRotten = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Moisture")
    float Moisture = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Moisture")
    float MaxMoisture = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Moisture")
    float MoistureLossPerTick = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Health")
    float PlantHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Health")
    float MaxPlantHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Health")
    float HealthLossWhenDry = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Rot")
    float TimeAfterReady = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Rot")
    float TimeBeforeRotAfterReady = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Tick")
    float PlantTickTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Pull")
    bool bIsBeingPulled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Pull")
    float PullProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Pull")
    float RequiredPullProgress = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Pull")
    float PullPowerPerSecond = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    bool bShowRootAboveGroundForDebug = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    float RootVisibleZ = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    float RootUndergroundZ = -25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    FVector LeavesMinScale = FVector(0.2f, 0.2f, 0.2f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    FVector LeavesMaxScale = FVector(1.0f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    FVector RootMinScale = FVector(0.3f, 0.3f, 0.3f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant|Visual")
    FVector RootMaxScale = FVector(5.0f, 5.0f, 5.0f);

    UFUNCTION(BlueprintCallable, Category = "Plant")
    void PlantTick();

    UFUNCTION(BlueprintCallable, Category = "Plant")
    void WaterPlant(float WaterAmount);

    UFUNCTION(BlueprintCallable, Category = "Plant")
    void MarkRotten();

    UFUNCTION(BlueprintCallable, Category = "Plant")
    void UpdatePlantVisual();

    UFUNCTION(BlueprintCallable, Category = "Plant|Pull")
    bool CanPull() const;

    UFUNCTION(BlueprintCallable, Category = "Plant|Pull")
    void BeginPull();

    UFUNCTION(BlueprintCallable, Category = "Plant|Pull")
    bool ContinuePull(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Plant|Pull")
    void CancelPull();

    UFUNCTION(BlueprintCallable, Category = "Plant|Pull")
    void FinishPull();

private:
    FTimerHandle PlantTickTimerHandle;
};