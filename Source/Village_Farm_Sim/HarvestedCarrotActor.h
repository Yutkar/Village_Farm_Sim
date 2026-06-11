#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "HarvestedCarrotActor.generated.h"

UCLASS()
class VILLAGE_FARM_SIM_API AHarvestedCarrotActor : public AActor
{
    GENERATED_BODY()

public:
    AHarvestedCarrotActor();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCanBeGrabbed = true;
};