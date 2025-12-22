#include "Game/Interfaces/Interactable.h"

void IInteractable::NativeInteract(AActor* contextActor)
{
	BP_OnInteract(contextActor);
}
