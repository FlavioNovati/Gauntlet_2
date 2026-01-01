#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Subsystems/PoolerSubSystem/ObjectPoolerSubsystem.h"
#include "Game/Subsystems/PoolerSubSystem/PoolEnablingData.h"
#include "Spawner.generated.h"

UCLASS()
class GAUNTLET_2_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ActorClassToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 InitialPoolSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandler;

	void RelaseCanSpawn();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	bool CanSpawn = true;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AActor* Spawn(FPoolEnablingData spawnData);

};
