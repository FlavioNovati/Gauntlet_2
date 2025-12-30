#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Interfaces/Interactable.h"
#include "Game/Components/Interaction/InteractionDebugData.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAUNTLET_2_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Radious of the interaction", AllowPrivateAccess = true, Category = "Interaction"))
	float InteractionRadius = 150.f;

	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Offset of the interaction sphere", AllowPrivateAccess = true, Category = "Interaction"))
	FVector InteractionOffset = FVector::Zero();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction"))
	bool ExcludeDisabledActors = true;

	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Array of all Interactables Actors that are near the player", AllowPrivateAccess = true, Category = "Interaction"))
	TArray<AActor*> NearbyInteractablesActors;

	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Array of all nearby Interactables", AllowPrivateAccess = true, Category = "Interaction"))
	TArray<TScriptInterface<IInteractable>> NearbyInteractables;

	UPROPERTY(BlueprintReadOnly, meta = (Tooltip = "Closest Interactable", AllowPrivateAccess = true, Category = "Interaction"))
	TScriptInterface<IInteractable> ClosestInteractable;

#pragma region Debug

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug"))
	bool ShowDebugInfo = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug", EditCondition = "ShowDebugInfo"))
	FInteractionDebugData NearbyActorsDebugData = FInteractionDebugData(15.f, FColor::Red, true);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug", EditCondition = "ShowDebugInfo"))
	FInteractionDebugData NearbyInteractablesDebugData = FInteractionDebugData(15.f, FColor::Emerald, true);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug", EditCondition = "ShowDebugInfo"))
	FInteractionDebugData ClosestInteractableDebugData = FInteractionDebugData(15.f, FColor::Yellow, true);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug", EditCondition = "ShowDebugInfo"))
	FColor InteractionDebugColor = FColor::Purple;

	void DrawDebug(float drawTime, TArray<AActor*> nearbyActors, TArray<TScriptInterface<IInteractable>> nearbyInteractablesActors, TScriptInterface<IInteractable> closestInteractable);
	void HighlightActorsDebug(float drawTime, TArray<AActor*> actorsToHighlight, FInteractionDebugData debugData);
	void HighlightActorDebug(float drawTime, AActor* actorToHighlight, FInteractionDebugData debugData);
	TArray<AActor*> FromTPointerToActor(TArray<TScriptInterface<IInteractable>> pointerArray);

#pragma endregion

	TArray<TScriptInterface<IInteractable>> GetInteractablesFromActors(const TArray<AActor*> actors);
	TArray<AActor*> GetOverlappingActors(float deltaTimeContext);
	TScriptInterface<IInteractable> GetClosest(const TArray<TScriptInterface<IInteractable>> interactables);
	bool ImplementsInteractable(AActor* actorToCheck);

	void Interact(IInteractable* interactable);
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, meta = ( Category = "Interaction") )
	void Interact();
};
