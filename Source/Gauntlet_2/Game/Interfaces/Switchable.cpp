#include "Game/Interfaces/Switchable.h"

void ISwitchable::NativeSwitch()
{
	//Reverse switch state
	bSwitchState = !bSwitchState;

	//Check updated Switch state
	if (bSwitchState)
	{
		//Call C++ implementation
		NativeSwitchON();
		//Call Blueprint implementation
		BP_OnSwitchON();
	}
	else
	{
		//Call C++ implementation
		NativeSwitchOFF();
		//Call Blueprint implementation
		BP_OnSwitchOFF();
	}
}
