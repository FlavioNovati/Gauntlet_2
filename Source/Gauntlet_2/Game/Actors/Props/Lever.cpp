#include "Game/Actors/Props/Lever.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	for (AActor* actor : LeverTargets)
	{
		//Clean nullptr
		//Clean actors that does not implements the interface
	}
	Super::BeginPlay();
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	//Call On Interact function BP side
	IInteractable::Execute_BP_OnInteract(this, contextActor);
}
#pragma endregion

