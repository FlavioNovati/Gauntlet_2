#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class GAUNTLET_2_API IDamageable
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void BP_OnDamage(float amount);

	/* C++ Implementation of Damage */
	virtual void NativeDamage(float amount) = 0;

public:

	/* C++ side damage function */
	void Damage(float amount) { NativeDamage(amount); };
};
