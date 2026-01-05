#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Interactable.h"
#include "Game/Interfaces/Switchable.h"
#include "Materials/MaterialInterface.h"
#include "Lever.generated.h"

UCLASS()
class GAUNTLET_2_API ALever : public AActor, public IInteractable
{
	GENERATED_BODY()

public:	
	ALever();

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	UMaterialInterface* LeverBodyMaterialParent;
	UMaterialInstanceDynamic* LeverDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor OnColor = FColor::Green;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor OffColor = FColor::Red;


#if WITH_EDITOR
	//Set Dynamic Material And Update color both in scene and in BP editor

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	void OnConstruction(const FTransform& Transform);
#endif

	//Create Root
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeverBody;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* HandlePivot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeverHandle;

protected:	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void SwitchAllTargets();

	void InvertLeverState();

	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void OnLeverOn();

	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void OnLeverOff();


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true, Category = "Interaction"))
	bool bCanInteract = true;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true, Category = "Lever"))
	bool bLeverState = false;

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

	TArray<TScriptInterface<ISwitchable>> SwitchableTargets;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region IInteractable Interface Implementation

	bool NativeCanInteract(AActor* contextActor) override;

	void NativeInteract(AActor* contextActor) override;
#pragma endregion
};
