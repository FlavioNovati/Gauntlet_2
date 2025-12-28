#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class GAUNTLET_2_API IInteractable
{
	GENERATED_BODY()	

public:
	//Can Interact Implementation
	virtual bool NativeCanInteract(AActor* contextActor) = 0;

	//Return true if the Interface can be interacted
	bool CanInteract(AActor* contextActor) { return NativeCanInteract(contextActor); };

	/* Function that gets called when Interact gets called via C++ */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnInteract(AActor* contextActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	bool BP_CanInteract();

	/* Default c++ implementation */
	virtual void NativeInteract(AActor* contextActor) = 0;

	/* C++ callable */
	virtual void Interact(AActor* contextActor) { NativeInteract(contextActor); };
};
