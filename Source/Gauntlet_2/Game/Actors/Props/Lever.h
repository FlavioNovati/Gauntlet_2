// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Interactable.h"
#include "Game/Interfaces/Switchable.h"
#include "Lever.generated.h"

UCLASS()
class GAUNTLET_2_API ALever : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ALever();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCanInteract = true;

	/*Array of targets of the lever, 
	upon interaction all Target will be Switch On or Off
	(Target Must Implement ISwitchable)*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere,	meta = (
		AllowPrivateAccess = true,
		MustImplement = "/Script/Gauntlet_2.Switchable",
		Category = "Lever",
		ToolTip = "Array of targets of the lever, upon interaction all Target will be Switch On or Off (Target Must Implement ISwitchable)"
	))
	TArray<AActor*> LeverTargets;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region IInteractable Interface Implementation

	bool NativeCanInteract(AActor* contextActor) override;

	void NativeInteract(AActor* contextActor) override;
#pragma endregion

};
