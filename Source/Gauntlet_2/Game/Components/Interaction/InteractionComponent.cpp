#include "Game/Components/Interaction/InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Get Close Actors
	TArray<AActor*> nearbyActors = GetOverlappingActors(DeltaTime);
	//Get Interactables Actors from nearby actors and update list
	NearbyInteractables = GetInteractablesFromActors(nearbyActors);
	//Get Closest
	ClosestInteractable = GetClosest(NearbyInteractables);

	//Draw Debug
	if(ShowDebugInfo)
		DrawDebug(DeltaTime, nearbyActors, NearbyInteractablesActors, ClosestInteractable);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionComponent::Interact()
{
	//No Interactable nearby
	if (ClosestInteractable.GetObject() == nullptr)
		return;

	//Get Actor
	AActor* actorToInteract = Cast<AActor>(ClosestInteractable.GetObject());
	//C++ implemented
	IInteractable* interactable = ClosestInteractable.GetInterface();
	//Blueprint implemented -> Interact
	if (interactable == nullptr)
		interactable = Cast<IInteractable>(actorToInteract);
	
	Interact(interactable);
}

void UInteractionComponent::Interact(IInteractable* interactable)
{
	//Validate Interactable
	if (interactable == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("- ERROR WHILE INTERACTING -"));
			return;
		}
	}

	//Check if the interactable can be interacted
	if(interactable->CanInteract(GetOwner()))
		interactable->Interact(GetOwner());
}

TArray<AActor*> UInteractionComponent::GetOverlappingActors(float deltaTimeContext)
{
	TArray<AActor*> overlappingActors;
	overlappingActors.Init(nullptr, 0);

	TArray<FHitResult> outHits;
	FVector sweepLocation = GetOwner()->GetActorLocation();
	sweepLocation += InteractionOffset;

	FCollisionShape interactionShape = FCollisionShape::MakeSphere(InteractionRadius);

	//Set Hit variable for later check
	bool bIsHit = GetWorld()->SweepMultiByChannel(outHits, sweepLocation, sweepLocation, FQuat::Identity, ECC_WorldStatic, interactionShape);
	
	//Check if something got hit
	if (bIsHit)
	{
		//-> Cycle throught all hittet Objects
		for (FHitResult Hit : outHits)
		{
			//Check if need to exclude
			if (ExcludeDisabledActors && Hit.GetActor()->IsActorTickEnabled())
			{
				overlappingActors.Add(Hit.GetActor());
			}
			else
			{
				//Add actor to nearby Interactables
				overlappingActors.Add(Hit.GetActor());
			}
		}
	}

	overlappingActors.Remove(GetOwner());

	return overlappingActors;
}

TScriptInterface<IInteractable> UInteractionComponent::GetClosest(const TArray<TScriptInterface<IInteractable>> interactables)
{
	if (interactables.Num() <= 0)
		return TScriptInterface<IInteractable>(nullptr);

	//Distance with some clearence
	float closestDist = InteractionRadius + 5.f;
	AActor* closestActor;
	FVector location = GetOwner()->GetActorLocation() + InteractionOffset;


	for (TScriptInterface<IInteractable> interactable : interactables)
	{
		if (interactable.GetObject() == nullptr)
			continue;

		AActor* interactableActor = Cast<AActor>(interactable.GetObject());
		if (interactableActor == nullptr)
			continue;

		FVector actorLocation = interactableActor->GetActorLocation();
		float dirstanceFromObject = FVector::Distance(location, actorLocation);

		if (dirstanceFromObject < closestDist)
		{
			closestActor = interactableActor;
			closestDist = dirstanceFromObject;
		}
	}
	
	return TScriptInterface<IInteractable>(closestActor);
}

TArray<TScriptInterface<IInteractable>> UInteractionComponent::GetInteractablesFromActors(const TArray<AActor*> actors)
{
	TArray<TScriptInterface<IInteractable>> tInteractables = TArray<TScriptInterface<IInteractable>>();
	
	for (AActor* actorToCheck : actors)
	{
		if (actorToCheck == nullptr)
			continue;

		if (ImplementsInteractable(actorToCheck))
			tInteractables.Add(TScriptInterface<IInteractable>(actorToCheck));
	}

	return tInteractables;
}

bool UInteractionComponent::ImplementsInteractable(AActor* actorToCheck)
{
	//Check Existance
	if (actorToCheck == nullptr)
		return false;

	TScriptInterface<IInteractable> tInteractable = TScriptInterface<IInteractable>(actorToCheck);

	//C++ implemented side interface
	if(tInteractable.GetInterface())
		return true;

	if (Cast<IInteractable>(actorToCheck))
		return true;

	return false;
}


#pragma region Debug

void UInteractionComponent::DrawDebug(float drawTime, TArray<AActor*> nearbyActors, TArray<AActor*> nearbyInteractablesActors, TScriptInterface<IInteractable> closestInteractable)
{
	//Draw nearby Actors
	HighlightActorDebug(drawTime, nearbyActors, NearbyActorsDebugData);

	//Draw nearby Interactables
	HighlightActorDebug(drawTime, nearbyInteractablesActors, NearbyInteractablesDebugData);
	
	//Draw closest Interactable
	HighlightActorDebug(drawTime, Cast<AActor>(closestInteractable.GetObject()), ClosestInteractableDebugData);

	//Draw Interaction sphere
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation() + InteractionOffset, InteractionRadius, 15, InteractionDebugColor, false);
}

void UInteractionComponent::HighlightActorDebug(float drawTime, TArray<AActor*> actorsToHighlight, FInteractionDebugData debugData)
{
	for (AActor* nearbyActor : actorsToHighlight)
		HighlightActorDebug(drawTime, nearbyActor, debugData);
}

void UInteractionComponent::HighlightActorDebug(float drawTime, AActor* actorToHighlight, FInteractionDebugData debugData)
{
	if (actorToHighlight == nullptr)
		return;

	UWorld* world = GetWorld();
	FVector targetActorLocation = actorToHighlight->GetActorLocation();

	DrawDebugSphere(GetWorld(), targetActorLocation, debugData.DebugSphereRadius, 15, debugData.DebugColor, false);
	
	if (debugData.ShowDebugLine)
		DrawDebugLine(world, GetOwner()->GetActorLocation() + InteractionOffset, targetActorLocation, debugData.DebugColor, false);
}

#pragma endregion

/*
if (!tInteractable.GetInterface()->CanInteract(actorToCheck))
			return false;

tInteractable.GetInterface()->Interact(actorToCheck);
*/

