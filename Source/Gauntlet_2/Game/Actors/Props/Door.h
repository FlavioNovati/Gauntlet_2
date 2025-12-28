#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interfaces/Switchable.h"
#include "Door.generated.h"

UCLASS()
class GAUNTLET_2_API ADoor : public AActor, public ISwitchable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NativeSwitchON() override;
	virtual void NativeSwitchOFF() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
