#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Subsystems/PoolerSubsystem/Poollable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
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

	virtual void Disable() override;
	virtual void Enable(FPoolEnablingData enablingData) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USphereComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	float ProjectileSpeed = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	float ProjectileDamage = 10.0f;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
