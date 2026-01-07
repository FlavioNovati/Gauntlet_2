#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/Interfaces/Interactable.h"
#include "Game/Interfaces/PickableTarget.h"
#include "Pickable.generated.h"

UINTERFACE(MinimalAPI)
class UPickable : public UInteractable
{
	GENERATED_BODY()
};

class GAUNTLET_2_API IPickable : public IInteractable
{
	GENERATED_BODY()

public:
	virtual void Pickup(TScriptInterface<IPickableTarget> PickableTarget) = 0;
	virtual void RelasePickable(TScriptInterface<IPickableTarget> PickableTarget) = 0;
	virtual void ResetPickable() = 0;
};
