// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Actors/Props/MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformPivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	RootComponent = PlatformPivot;

	//Create Dynamic material if missing
	if (PlatformBodyMaterialParent && !PlatformDynamicMaterial)
	{
		PlatformDynamicMaterial = UMaterialInstanceDynamic::Create(PlatformBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));
		PlatformDynamicMaterial->SetVectorParameterValue("Base Color", InactiveColor);
	}
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//Create Dynamic material if missing
	if (PlatformBodyMaterialParent && !PlatformDynamicMaterial)
	{
		PlatformDynamicMaterial = UMaterialInstanceDynamic::Create(PlatformBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));
		PlatformDynamicMaterial->SetVectorParameterValue("Base Color", InactiveColor);
	}

	PlatformPivot->GetChildrenComponents(true, Components);

	for (USceneComponent* component : Components)
	{
		ComponentsToMove.Add(FComponentToMove(component));

		//Check if the component is a static mesh
		UStaticMeshComponent* staticMesh = Cast<UStaticMeshComponent>(component);
		if (staticMesh)
		{
			staticMesh->SetMaterial(0, PlatformDynamicMaterial);
		}
	}

	SetActorTickEnabled(false);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePivot(DeltaTime);
}

void AMovingPlatform::MovePivot(float deltaTime)
{
	FRotator DeltaRotation = FRotator(0.f, RotationSpeed * deltaTime, 0.f); 
	PlatformPivot->AddLocalRotation(DeltaRotation);

	UpdateComponentRotation(deltaTime);
}

void AMovingPlatform::UpdateComponentRotation(float deltaTime)
{
	for (FComponentToMove ctm : ComponentsToMove)
	{
		USceneComponent* component = ctm.Component;
		component->SetWorldRotation(ctm.StartRotator);
	}
}

void AMovingPlatform::NativeSwitchON()
{
	PlatformDynamicMaterial->SetVectorParameterValue("Base Color", ActiveColor);
	SetActorTickEnabled(true);
}

void AMovingPlatform::NativeSwitchOFF()
{
	PlatformDynamicMaterial->SetVectorParameterValue("Base Color", InactiveColor);
	SetActorTickEnabled(false);
}

