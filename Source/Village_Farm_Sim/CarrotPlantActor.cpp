#include "CarrotPlantActor.h"
#include "SoilPlotActor.h"
#include "HarvestedCarrotActor.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACarrotPlantActor::ACarrotPlantActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    LeavesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeavesMesh"));
    LeavesMesh->SetupAttachment(SceneRoot);

    RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
    RootMesh->SetupAttachment(SceneRoot);

    RottenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RottenMesh"));
    RottenMesh->SetupAttachment(SceneRoot);

    LeavesMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RottenMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Tags.Add(TEXT("CarrotPlant"));
}

void ACarrotPlantActor::BeginPlay()
{
    Super::BeginPlay();

    Moisture = FMath::Clamp(Moisture, 0.0f, MaxMoisture);
    PlantHealth = FMath::Clamp(PlantHealth, 0.0f, MaxPlantHealth);

    UpdatePlantVisual();

    if (PlantTickTime > 0.0f)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimer(
                PlantTickTimerHandle,
                this,
                &ACarrotPlantActor::PlantTick,
                PlantTickTime,
                true
            );
        }
    }
}

void ACarrotPlantActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    UpdatePlantVisual();
}

void ACarrotPlantActor::PlantTick()
{
    if (bIsRotten)
    {
        return;
    }

    Moisture = FMath::Clamp(Moisture - MoistureLossPerTick, 0.0f, MaxMoisture);

    if (bIsReadyToHarvest)
    {
        TimeAfterReady += PlantTickTime;

        if (TimeAfterReady >= TimeBeforeRotAfterReady)
        {
            MarkRotten();
        }

        return;
    }

    if (Moisture > 0.0f)
    {
        GrowthProgress += GrowthPerTick;

        while (GrowthProgress >= 100.0f && GrowthStage < MaxGrowthStage)
        {
            GrowthProgress -= 100.0f;
            GrowthStage++;
        }

        if (GrowthStage >= MaxGrowthStage)
        {
            GrowthStage = MaxGrowthStage;
            GrowthProgress = 100.0f;
            bIsReadyToHarvest = true;
            TimeAfterReady = 0.0f;
        }

        UpdatePlantVisual();
    }
    else
    {
        PlantHealth = FMath::Clamp(PlantHealth - HealthLossWhenDry, 0.0f, MaxPlantHealth);

        if (PlantHealth <= 0.0f)
        {
            MarkRotten();
        }
    }
}

void ACarrotPlantActor::WaterPlant(float WaterAmount)
{
    if (bIsRotten || WaterAmount <= 0.0f)
    {
        return;
    }

    Moisture = FMath::Clamp(Moisture + WaterAmount, 0.0f, MaxMoisture);
}

void ACarrotPlantActor::MarkRotten()
{
    bIsRotten = true;
    bIsReadyToHarvest = false;
    bIsBeingPulled = false;

    UpdatePlantVisual();
}

void ACarrotPlantActor::UpdatePlantVisual()
{
    if (!RootMesh || !RottenMesh)
    {
        return;
    }

    if (bIsRotten)
    {
        if (LeavesMesh)
        {
            LeavesMesh->SetVisibility(true);
            LeavesMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.08f));
        }

        RootMesh->SetVisibility(false);
        RottenMesh->SetVisibility(true);

        RottenMesh->SetRelativeScale3D(RootMaxScale * 0.8f);

        const float RottenZ = bShowRootAboveGroundForDebug ? RootVisibleZ : RootUndergroundZ;
        RottenMesh->SetRelativeLocation(FVector(0.0f, 0.0f, RottenZ));

        return;
    }

    RottenMesh->SetVisibility(false);

    if (LeavesMesh)
    {
        LeavesMesh->SetVisibility(true);

        const float StageAlphaForLeaves = MaxGrowthStage > 0
            ? static_cast<float>(GrowthStage) / static_cast<float>(MaxGrowthStage)
            : 0.0f;

        const FVector LeavesScale = FMath::Lerp(LeavesMinScale, LeavesMaxScale, StageAlphaForLeaves);
        LeavesMesh->SetRelativeScale3D(LeavesScale);
        LeavesMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }

    const float StageAlpha = MaxGrowthStage > 0
        ? static_cast<float>(GrowthStage) / static_cast<float>(MaxGrowthStage)
        : 0.0f;

    if (GrowthStage <= 0)
    {
        RootMesh->SetVisibility(false);
        return;
    }

    RootMesh->SetVisibility(true);

    const FVector RootScale = FMath::Lerp(RootMinScale, RootMaxScale, StageAlpha);
    RootMesh->SetRelativeScale3D(RootScale);

    float RootZ = bShowRootAboveGroundForDebug ? RootVisibleZ : RootUndergroundZ;

    if (bIsBeingPulled)
    {
        const float PullAlpha = RequiredPullProgress > 0.0f
            ? PullProgress / RequiredPullProgress
            : 0.0f;

        RootZ += PullAlpha * 40.0f;
    }

    RootMesh->SetRelativeLocation(FVector(0.0f, 0.0f, RootZ));
}

bool ACarrotPlantActor::CanPull() const
{
    return bIsReadyToHarvest && !bIsRotten && HarvestedCarrotClass != nullptr;
}

void ACarrotPlantActor::BeginPull()
{
    if (!CanPull())
    {
        return;
    }

    bIsBeingPulled = true;
}

bool ACarrotPlantActor::ContinuePull(float DeltaTime)
{
    if (!CanPull())
    {
        CancelPull();
        return false;
    }

    bIsBeingPulled = true;

    PullProgress += PullPowerPerSecond * DeltaTime;
    PullProgress = FMath::Clamp(PullProgress, 0.0f, RequiredPullProgress);

    UpdatePlantVisual();

    if (PullProgress >= RequiredPullProgress)
    {
        FinishPull();
        return true;
    }

    return false;
}

void ACarrotPlantActor::CancelPull()
{
    bIsBeingPulled = false;
    PullProgress = 0.0f;

    UpdatePlantVisual();
}

void ACarrotPlantActor::FinishPull()
{
    if (!CanPull())
    {
        return;
    }

    UWorld* World = GetWorld();

    if (World)
    {
        const FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
        const FRotator SpawnRotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();

        World->SpawnActor<AHarvestedCarrotActor>(
            HarvestedCarrotClass,
            SpawnLocation,
            SpawnRotation,
            SpawnParams
        );
    }

    if (OwnerSoil)
    {
        OwnerSoil->ClearPlant();
    }

    Destroy();
}