#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "WellActor.generated.h"

class AWateringCanActor;

UCLASS()
class VILLAGE_FARM_SIM_API AWellActor : public AActor
{
    GENERATED_BODY()

public:
    AWellActor();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UFUNCTION(BlueprintCallable, Category = "Well")
    bool FillWateringCan(AWateringCanActor* WateringCan);
};