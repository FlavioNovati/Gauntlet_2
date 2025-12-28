#include "Game/Interfaces/Interactable.h"

bool IInteractable::NativeCanInteract(AActor* contextActor)
{
	return IInteractable::Execute_BP_CanInteract(contextActor);
}

void IInteractable::NativeInteract(AActor* contextActor)
{
	BP_OnInteract(contextActor);
}
