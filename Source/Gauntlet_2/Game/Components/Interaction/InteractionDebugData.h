#pragma once

#include "CoreMinimal.h"
#include "InteractionDebugData.generated.h"

USTRUCT(BlueprintType)
struct FInteractionDebugData
{
	GENERATED_BODY()

public:
	FInteractionDebugData();
	FInteractionDebugData(float radius, FColor color, bool showDebugLine);

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug"))
	float DebugSphereRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug"))
	FColor DebugColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true, Category = "Interaction Debug"))
	bool ShowDebugLine;
};
