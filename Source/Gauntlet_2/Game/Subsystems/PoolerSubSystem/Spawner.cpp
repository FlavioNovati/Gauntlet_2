#include "Game/Subsystems/PoolerSubSystem/Spawner.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TimerHandler = FTimerHandle();
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	//Create Pool
	UObjectPoolerSubsystem* poolSubsystem = GetWorld()->GetSubsystem<UObjectPoolerSubsystem>();
	if (poolSubsystem)
	{
		poolSubsystem->CreateNewPool(ActorClassToSpawn, InitialPoolSize);
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* ASpawner::Spawn(FPoolEnablingData spawnData)
{
	if (!CanSpawn)
		return nullptr;

	//Create Pool
	UObjectPoolerSubsystem* poolSubsystem = GetWorld()->GetSubsystem<UObjectPoolerSubsystem>();
	if (poolSubsystem)
	{
		TScriptInterface<IPoollable> actorInterface = poolSubsystem->GetObjectFromPool(ActorClassToSpawn);
		AActor* actorToSpawn = Cast<AActor>(actorInterface.GetObject());
		
		if (!actorInterface)
		{
			IPoollable::Execute_BP_Enable(actorToSpawn, spawnData);
		}
		else
		{
			actorInterface.GetInterface()->NativeEnable(spawnData);
		}

		CanSpawn = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASpawner::RelaseCanSpawn, 1.0f, true);
		 
		return actorToSpawn;
	}

	return nullptr;
}

void ASpawner::RelaseCanSpawn()
{
	CanSpawn = true;
}

