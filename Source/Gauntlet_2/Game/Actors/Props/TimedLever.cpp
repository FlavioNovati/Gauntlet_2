#include "Game/Actors/Props/TimedLever.h"

bool ATimedLever::NativeCanInteract(AActor* contextActor)
{
	return bCanInteract;
}

void ATimedLever::NativeInteract(AActor* contextActor)
{
	//check if actor exists
	if (contextActor == nullptr)
		return;

	bCanInteract = false;
	GetWorld()->GetTimerManager().SetTimer(FlickLeverTimerHandle, this, &ATimedLever::ResetLever, ResetFlickTime, false);

	InvertLeverState();

	//Call On Interact function BP side
	IInteractable::Execute_BP_OnInteract(this, contextActor);
}

void ATimedLever::ResetLever()
{
	InvertLeverState();

	bCanInteract = true;
}
