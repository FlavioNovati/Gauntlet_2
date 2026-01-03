#include "Game/Subsystems/PoolerSubsystem/ObjectPoolerSubsystem.h"

UObjectPoolerSubsystem::UObjectPoolerSubsystem()
{
	PoolingMap = TMap<TSubclassOf<AActor>, FPoolData>();
}

void UObjectPoolerSubsystem::CreateNewPool(TSubclassOf<AActor> poolActorClass, int32 initialSize)
{
	//Level is not loaded
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get oject from pool: WORLD DOES NOT EXIST!"));
		return;
	}

	//Class does not implements interface
	if (!poolActorClass->ImplementsInterface(UPoollable::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get oject from pool: CLASS DOES NOT IMPLEMENTS IPoollable!"));
		return;
	}

	//Create Spawn Parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Check if the class already has a pool -> Add AActors to Pool
	if (PoolingMap.Contains(poolActorClass))
	{
		//Get a reference to the pool
		FPoolData* poolData = PoolingMap.Find(poolActorClass);

		//Spawn actors to use
		for (int i = 0; i < initialSize; i++)
		{
			//Spawn objects to add in the pool
			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(poolActorClass, SpawnParameters);

			IPoollable* poollable = Cast<IPoollable>(spawnedActor);

			//Add the spawned actor to the PoolData
			poolData->UsablePoolingObjects.AddUnique(spawnedActor);

			if (poollable)
				poollable->Disable();
			else
				IPoollable::Execute_BP_Disable(spawnedActor);
		}

		//End
		return;
	}

	//Create New Pool
	FPoolData newPoolData = FPoolData();

	//Spawn initial actors to use
	for (int i = 0; i < initialSize; i++)
	{
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(poolActorClass, SpawnParameters);
		IPoollable* poollable = Cast<IPoollable>(spawnedActor);

		//Add the spawned actor to the PoolData
		newPoolData.UsablePoolingObjects.AddUnique(spawnedActor);

		if (poollable)
			poollable->Disable();
		else
			IPoollable::Execute_BP_Disable(spawnedActor);
	}

	//Add Object To Map
	PoolingMap.Add(poolActorClass, newPoolData);
}

TScriptInterface<IPoollable> UObjectPoolerSubsystem::GetObjectFromPool(TSubclassOf<AActor> actorClass)
{
	//Level is not loaded
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get oject from pool: WORLD DOES NOT EXIST!"));
		return nullptr;
	}

	//Does not have a pool of that class
	if (!PoolingMap.Contains(actorClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get oject from pool: CLASS NOT IN A POOL!"));
		return nullptr;
	}

	//Class does not implements interface
	if (!actorClass->ImplementsInterface(UPoollable::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get oject from pool: CLASS DOES NOT IMPLEMENTS IPOOLLABLE!"));
		return nullptr;
	}

	//Create Available Actor
	TScriptInterface<IPoollable> availableActor;
	//Get Pool
	FPoolData* pool = PoolingMap.Find(actorClass);

	//Check if pool have available entries -> Add Object To Pool
	if (pool->UsablePoolingObjects.IsEmpty())
	{
		//Spawn Actor
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		availableActor = GetWorld()->SpawnActor<AActor>(actorClass, spawnParameters);
		//Add Actor To Pool
		pool->EnabledPoolingObjects.AddUnique(availableActor);
	}
	//Available Actor
	else
	{
		availableActor = pool->UsablePoolingObjects[0];
		pool->UsablePoolingObjects.Remove(availableActor);
		pool->EnabledPoolingObjects.AddUnique(availableActor);
	}

	return availableActor;
}

bool UObjectPoolerSubsystem::ReturnObjectToPool(TSubclassOf<AActor> actorClass, TScriptInterface<IPoollable> actorToReturn)
{
	//Level is not loaded
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to add oject to pool: WORLD DOES NOT EXIST!"));
		return false;
	}

	//Does not have a pool of that class
	if (!PoolingMap.Contains(actorClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to add oject to pool: CLASS NOT IN A POOL!"));
		return false;
	}

	//Class does not implements interface
	if (!actorClass->ImplementsInterface(UPoollable::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to add oject to pool: CLASS DOES NOT IMPLEMENTS IPOOLLABLE!"));
		return false;
	}

	//Disable object to return to pool
	if (actorToReturn.GetInterface()) //Implemented C++ side
	{
		actorToReturn.GetInterface()->Disable();
	}
	else //Added via BP
	{
		IPoollable::Execute_BP_Disable(actorToReturn.GetObject());
	}

	// We get the object pool with our pool objects
	FPoolData* pool = PoolingMap.Find(actorClass);

	if (!pool->EnabledPoolingObjects.Contains(actorToReturn))
		UE_LOG(LogTemp, Warning, TEXT("Trying to add to the pool an object that wasn't registered before"));

	//Unregister from enables objects
	pool->EnabledPoolingObjects.Remove(actorToReturn);

	//Add Object to usable objects
	pool->UsablePoolingObjects.AddUnique(actorToReturn);

	return true;
}

FString UObjectPoolerSubsystem::ToString()
{
	FString output = "";

	//->GetClass()->GetFName().ToString() + " --\n";

	for (auto mapEntry = PoolingMap.begin(); mapEntry != PoolingMap.end(); ++mapEntry)
	{
		TSubclassOf<AActor> key = mapEntry.Key();
		output += "-- Pool Class Type: " + key->GetName() + " --\n";
		FPoolData* pool = PoolingMap.Find(key);
		output += pool->ToString();
		output += "\n\n";
	}

	return output;
}

