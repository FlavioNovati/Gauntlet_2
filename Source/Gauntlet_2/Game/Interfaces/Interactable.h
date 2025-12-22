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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool BP_CanInteract();

	virtual bool NativeCanInteract() = 0;

	bool CanInteract() { return NativeCanInteract(); };

	/* Function that gets called when Interact gets called via C++ */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnInteract(AActor* contextActor);

	/* Default c++ implementation */
	virtual void NativeInteract(AActor* contextActor) = 0;

	/* C++ callable */
	virtual void Interact(AActor* contextActor) { NativeInteract(contextActor); };
};
