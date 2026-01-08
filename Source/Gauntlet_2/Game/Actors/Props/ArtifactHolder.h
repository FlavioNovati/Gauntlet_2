#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Interactable.h"
#include "Game/Interfaces/PickableTarget.h"
#include "ArtifactHolder.generated.h"

UCLASS()
class GAUNTLET_2_API AArtifactHolder : public AActor, public IInteractable, public IPickableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtifactHolder();
	
private:
	AActor* Artifact;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* ArtefactHolder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ArtefactHoldeMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	UMaterialInterface* ArtefactBodyMaterialParent;
	UMaterialInstanceDynamic* ArtefactDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor FullColor = FColor::Green;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor EmptyColor = FColor::Red;

#if WITH_EDITOR
	//Set Dynamic Material And Update color both in scene and in BP editor

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	void OnConstruction(const FTransform& Transform);
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnArtfactHolded();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Inherited via IInteractable
	bool NativeCanInteract(AActor* contextActor) override;

	void NativeInteract(AActor* contextActor) override;


	// Inherited via IPickableTarget
	void PickupPickable(AActor* actorToPlace) override;

	FTransform GetPickableTargetTransform() override;

};
