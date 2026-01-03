#pragma once

#include "CoreMinimal.h"
#include "Gauntlet_2Character.h"
#include "GameFramework/Character.h"
#include "Game/Interfaces/Targetable.h"
#include "Game/Interfaces/Damageable.h"
#include "Game/Components/Interaction/InteractionComponent.h"
#include "GauntletCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UCLASS()
class GAUNTLET_2_API AGauntletCharacter : public AGauntlet_2Character, public ITargetable, public IDamageable
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

	UPROPERTY(BlueprintReadOnly, meta = (Category = "Gauntlet Character", AllowPrivateAccess = true))
	bool bCanRecieveDamage = true;

	UPROPERTY(BlueprintReadOnly, meta = (Category = "Gauntlet Character", AllowPrivateAccess = true, EditCondition = "bCanRecieveDamage"))
	float HP = 100.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float RespawnTime;

	FTimerHandle RespawnTimer;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDead OnPlayerDead;

	void Respawn();
	void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Inherited via IDamageable
	void NativeDamage(float amount) override;
};
