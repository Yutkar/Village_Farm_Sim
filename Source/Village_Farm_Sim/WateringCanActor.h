#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "WateringCanActor.generated.h"

UCLASS()
class VILLAGE_FARM_SIM_API AWateringCanActor : public AActor
{
    GENERATED_BODY()

public:
    AWateringCanActor();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
    float CurrentWater = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
    float MaxWater = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
    float WaterUsePerAction = 10.0f;

    UFUNCTION(BlueprintCallable, Category = "Water")
    void FillFull();

    UFUNCTION(BlueprintCallable, Category = "Water")
    float UseWater(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Water")
    bool HasWater() const;
};