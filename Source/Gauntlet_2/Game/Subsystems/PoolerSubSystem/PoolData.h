#pragma once

#include "CoreMinimal.h"
#include "Game/Subsystems/PoolerSubsystem/Poollable.h"
#include "PoolData.generated.h"

USTRUCT(BlueprintType)
struct GAUNTLET_2_API FPoolData
{
	GENERATED_BODY()
	
	// Array with the pointers of the USABLE objects IN this pool
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Struct")
	TArray<TScriptInterface<IPoollable>> UsablePoolingObjects;

	// Array with the pointers of the ACTIVE IN SCENE objects FROM this pool
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Struct")
	TArray<TScriptInterface<IPoollable>> EnabledPoolingObjects;

public:
	FString ToString();
};
