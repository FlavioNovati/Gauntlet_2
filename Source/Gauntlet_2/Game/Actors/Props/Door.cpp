#include "Game/Actors/Props/Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	if (!GetWorld())
		return;

	Super::BeginPlay();

	if (bStartOn)
		NativeSwitchON();
	else
		NativeSwitchOFF();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


#pragma region ISwitchable Interface implementation

void ADoor::NativeSwitchON()
{

}

void ADoor::NativeSwitchOFF()
{

}
#pragma endregion

