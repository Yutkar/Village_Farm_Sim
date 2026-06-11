#include "PhysicsDoor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

APhysicsDoor::APhysicsDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Создаем дверную коробку (корень)
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	RootComponent = DoorFrameMesh;

	// 2. Создаем дверь
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	DoorMesh->SetSimulatePhysics(true);
	DoorMesh->GetBodyInstance()->bGenerateWakeEvents = true;

	// 3. Создаем петлю
	DoorConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DoorConstraint"));
	DoorConstraint->SetupAttachment(RootComponent);

	// Связываем компоненты
	DoorConstraint->SetConstrainedComponents(DoorFrameMesh, NAME_None, DoorMesh, NAME_None);

	// Фиксируем линейное движение
	DoorConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	DoorConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	DoorConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

	// Настраиваем угловое вращение под UE5
	DoorConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	DoorConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	DoorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);

	// По умолчанию запираем петлю
	DoorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
}

void APhysicsDoor::BeginPlay()
{
	Super::BeginPlay();

	// При старте проверяем состояние замка
	if (bIsLocked)
	{
		DoorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	}
	else
	{
		DoorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	}
}

void APhysicsDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhysicsDoor::ToggleLock()
{
	bIsLocked = !bIsLocked;

	if (bIsLocked)
	{
		DoorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	}
	else
	{
		DoorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	}
}