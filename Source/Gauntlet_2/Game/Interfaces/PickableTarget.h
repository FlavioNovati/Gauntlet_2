#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickableTarget.generated.h"

UINTERFACE(MinimalAPI)
class UPickableTarget : public UInterface
{
	GENERATED_BODY()
};

class GAUNTLET_2_API IPickableTarget
{
	GENERATED_BODY()
	
public:
	virtual void PickupPickable(AActor* actorToPlace) = 0;
	virtual FTransform GetPickableTargetTransform() = 0;
};
