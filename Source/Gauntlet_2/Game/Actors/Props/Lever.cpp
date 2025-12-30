#include "Game/Actors/Props/Lever.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SwitchableTargets = TArray<TScriptInterface<ISwitchable>>();
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{	
	if (!GetWorld())
		return;

	//Check all Lever Target and cache the one who implements the interface
	for (AActor* targetActor : LeverTargets)
	{
		if(targetActor == nullptr)
			continue;

		ISwitchable* switchable = Cast<ISwitchable>(targetActor);
		if(!switchable)
			continue;

		//Create TScript Interface of type ISwitchable
		TScriptInterface<ISwitchable> switchableTScriptInterface = TScriptInterface<ISwitchable>();
		switchableTScriptInterface.SetObject(targetActor);
		switchableTScriptInterface.SetInterface(switchable);

		//Add Unique Switchable
		SwitchableTargets.AddUnique(switchableTScriptInterface);
	}

	Super::BeginPlay();
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALever::SwitchAllTargets()
{
	//Invoke Switch on all lever targets
	for (TScriptInterface<ISwitchable> targetTInterface : SwitchableTargets)
	{
		targetTInterface.GetInterface()->Switch();
	}
}

#pragma region IInteractable Interface implementation

bool ALever::NativeCanInteract(AActor* contextActor)
{
	//Call default implementation of CanInteract
	return IInteractable::NativeCanInteract(contextActor);
};

void ALever::NativeInteract(AActor* contextActor)
{
	//check if actor exists
	if (contextActor == nullptr)
		return;

	if (SwitchableTargets.Num() <= 0)
		return;

	SwitchAllTargets();

	//Call On Interact function BP side
	IInteractable::Execute_BP_OnInteract(this, contextActor);
}
#pragma endregion

