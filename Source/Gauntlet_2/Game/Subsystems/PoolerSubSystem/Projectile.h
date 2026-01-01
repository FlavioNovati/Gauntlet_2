#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Subsystems/PoolerSubsystem/Poollable.h"
#include "Projectile.generated.h"

UCLASS()
class GAUNTLET_2_API AProjectile : public AActor, public IPoollable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
