#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Pickable.h"
#include "Artefact.generated.h"

UCLASS()
class GAUNTLET_2_API AArtefact : public AActor, public IPickable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtefact();

private:

	FTransform StartingTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	UMaterialInterface* ArtefactBodyMaterialParent;
	UMaterialInstanceDynamic* ArtefactDynamicMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor PickedColor = FColor::Purple;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor RelasedColor = FColor::Yellow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeverBody;

#if WITH_EDITOR
	//Set Dynamic Material And Update color both in scene and in BP editor

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	void OnConstruction(const FTransform& Transform);
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bIsPickedUp = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Inherited via IInteractable
	bool NativeCanInteract(AActor* contextActor) override;

	void NativeInteract(AActor* contextActor) override;
	
	// Inherited via IPickable
	void Pickup(TScriptInterface<IPickableTarget> PickableTarget) override;

	void RelasePickable(TScriptInterface<IPickableTarget> PickableTarget) override;


	// Inherited via IPickable
	void ResetPickable() override;

};
