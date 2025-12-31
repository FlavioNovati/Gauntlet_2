#include "Game/Interfaces/Switchable.h"

void ISwitchable::NativeSwitch(AActor* contextActor)
{
	//Reverse switch state
	bSwitchState = !bSwitchState;

	//Check updated Switch state
	if (bSwitchState)
	{
		//Call C++ implementation
		NativeSwitchON();
		//Call Blueprint implementation
		ISwitchable::Execute_BP_OnSwitchON(contextActor);
	}
	else
	{
		//Call C++ implementation
		NativeSwitchOFF();
		//Call Blueprint implementation
		ISwitchable::Execute_BP_OnSwitchOFF(contextActor);
	}
}
