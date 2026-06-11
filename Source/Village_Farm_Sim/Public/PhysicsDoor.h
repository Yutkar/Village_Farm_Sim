#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsDoor.generated.h"

class UStaticMeshComponent;
class UPhysicsConstraintComponent;

UCLASS()
class VILLAGE_FARM_SIM_API APhysicsDoor : public AActor
{
	GENERATED_BODY()

public:
	APhysicsDoor();

protected:
	virtual void BeginPlay() override;

	// Переменная замка: true - закрыто, false - открыто. Видна и редактируется в Блупринтах.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	bool bIsLocked = true;

public:
	virtual void Tick(float DeltaTime) override;

	// Функция переключения замка. BlueprintCallable позволяет вызывать её из Event Graph.
	UFUNCTION(BlueprintCallable, Category = "Door Settings")
	void ToggleLock();

private:
	// 1. Дверная коробка (косяк)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorFrameMesh;

	// 2. Подвижная дверь
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	// 3. Физическая петля
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* DoorConstraint;
};