#include "Game/Subsystems/PoolerSubsystem/PoolData.h"

FString FPoolData::ToString()
{
	FString output = "";

	int usableCount = UsablePoolingObjects.Num();
	int enabledCount = EnabledPoolingObjects.Num();

	output += "- Usable Cout: " + FString::FromInt(usableCount);
	output += "\n- Enabled Cout: " + FString::FromInt(enabledCount);
	output += "\n- Total In Pool: " + FString::FromInt(usableCount + enabledCount);

	return output;
}
