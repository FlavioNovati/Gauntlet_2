#include "Game/Components/Interaction/InteractionDebugData.h"

FInteractionDebugData::FInteractionDebugData()
{
	DebugSphereRadius = 15.f;
	DebugColor = FColor::Red;
	ShowDebugLine = false;
}

FInteractionDebugData::FInteractionDebugData(float radius, FColor color, bool showLine)
{
	DebugSphereRadius = radius;
	DebugColor = color;
	ShowDebugLine = showLine;
}
