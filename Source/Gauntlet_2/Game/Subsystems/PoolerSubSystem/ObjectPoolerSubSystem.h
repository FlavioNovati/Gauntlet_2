#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Game/Subsystems/PoolerSubsystem/PoolData.h"
#include "Game/Subsystems/PoolerSubsystem/Poollable.h"

#include "ObjectPoolerSubsystem.generated.h"

UCLASS()
class GAUNTLET_2_API UObjectPoolerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UObjectPoolerSubsystem();

private:
		TMap<TSubclassOf<AActor>, FPoolData> PoolingMap;

public:
	UFUNCTION(BlueprintCallable)
	void CreateNewPool(TSubclassOf<AActor> poolActorClass, int32 initialSize);

	UFUNCTION(BlueprintCallable)
	TScriptInterface<IPoollable> GetObjectFromPool(TSubclassOf<AActor> actorClass);

	UFUNCTION(BlueprintCallable)
	bool ReturnObjectToPool(TSubclassOf<AActor> actorClass, TScriptInterface<IPoollable> actorToReturn);

	UFUNCTION(BlueprintCallable)
	FString ToString();
};