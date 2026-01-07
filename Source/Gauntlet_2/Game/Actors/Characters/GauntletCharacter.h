#pragma once

#include "CoreMinimal.h"
#include "Gauntlet_2Character.h"
#include "GameFramework/Character.h"
#include "Game/Interfaces/Targetable.h"
#include "Game/Interfaces/Damageable.h"
#include "Game/Components/Interaction/InteractionComponent.h"
#include "Game/Interfaces/PickableTarget.h"
#include "Components/SceneComponent.h"
#include "GauntletCharacter.generated.h"

class AGauntletCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDead, AGauntletCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPause);

UCLASS()
class GAUNTLET_2_API AGauntletCharacter : public AGauntlet_2Character, public ITargetable, public IDamageable, public IPickableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGauntletCharacter();

protected:
	
	UPROPERTY(EditAnywhere)
	UInteractionComponent* InteractionComponent;
	void Interact() { InteractionComponent->Interact(); }

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Category = "Gauntlet Character", AllowPrivateAccess = true))
	FName PickableTargetSocketName = "";
	
	AActor* PickedActor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Category = "Gauntlet Character", AllowPrivateAccess = true))
	bool bCanRecieveDamage = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Category = "Gauntlet Character", AllowPrivateAccess = true, EditCondition = "bCanRecieveDamage"))
	float HP = 100.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

	void Respawn();
	void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Inherited via IDamageable
	void NativeDamage(float amount) override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDead OnPlayerDead;

	UPROPERTY(BlueprintAssignable)
	FOnPause OnPauseRequest;

	AActor* GetPickedActor() { return PickedActor; }

	void RemovePickedActor();

	// Inherited via IPickableTarget
	void PickupPickable(AActor* actorToPlace) override;

	// Inherited via IPickableTarget
	FTransform GetPickableTargetTransform() override;
};
