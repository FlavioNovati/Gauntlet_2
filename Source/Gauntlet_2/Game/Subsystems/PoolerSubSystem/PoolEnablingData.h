#pragma once

#include "CoreMinimal.h"
#include "PoolEnablingData.generated.h"

USTRUCT(BlueprintType)
struct GAUNTLET_2_API FPoolEnablingData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Enabling Data")
	FTransform ObjectPoolTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Enabling Data")
	FName ObjectPoolName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Enabling Data")
	FString ObjectPoolID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Enabling Data")
	FText ObjectPoolText;
};
