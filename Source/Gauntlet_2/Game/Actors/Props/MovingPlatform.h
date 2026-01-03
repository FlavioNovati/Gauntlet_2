#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Switchable.h"
#include "MovingPlatform.generated.h"

UCLASS()
class GAUNTLET_2_API AMovingPlatform : public AActor, public ISwitchable
{
	GENERATED_BODY()
	
	struct FComponentToMove
	{
	public:
		FComponentToMove() { Component = nullptr; }

		FComponentToMove(USceneComponent* component)
		{
			Component = component;
			StartRotator = Component->GetComponentRotation();
		}

		USceneComponent* Component;
		FRotator StartRotator;
	};

public:
	AMovingPlatform();

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	UMaterialInterface* PlatformBodyMaterialParent;
	UMaterialInstanceDynamic* PlatformDynamicMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true) )
	USceneComponent* PlatformPivot;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	float RotationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	FColor ActiveColor = FColor::Green;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	FColor InactiveColor = FColor::Blue;

	TArray<USceneComponent*> Components;
	TArray<FComponentToMove> ComponentsToMove;

	void MovePivot(float deltaTime);
	void UpdateComponentRotation(float deltaTime);

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Inherited via ISwitchable
	void NativeSwitchON() override;
	void NativeSwitchOFF() override;

};