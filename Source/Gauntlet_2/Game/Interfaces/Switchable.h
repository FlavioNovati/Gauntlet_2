#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Switchable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchCallback);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USwitchable : public UInterface
{
	GENERATED_BODY()
};


class GAUNTLET_2_API ISwitchable
{
	GENERATED_BODY()
	
private:
	/* State of the Switchable */
	bool bSwitchState = false;
	/* C++ base Switch */
	void NativeSwitch();

protected:
	virtual void NativeSwitchON() = 0;
	virtual void NativeSwitchOFF() = 0;

	/* ON implementation blueprint side */
	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true) )
	void BP_OnSwitchON();

	/* OFF implementation blueprint side */
	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true) )
	void BP_OnSwitchOFF();

public:	
	FORCEINLINE bool GetSwitchState() { return bSwitchState; };
	
	/* Switch Between the ON and OFF implementation */
	void Switch() { NativeSwitch(); };
};
