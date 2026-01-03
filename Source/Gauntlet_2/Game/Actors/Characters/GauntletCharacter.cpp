#include "Game/Actors/Characters/GauntletCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"

// Sets default values
AGauntletCharacter::AGauntletCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("Interaction Component");
}

// Called when the game starts or when spawned
void AGauntletCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGauntletCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGauntletCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Interact
		enhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGauntletCharacter::Interact);
	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGauntletCharacter::NativeDamage(float amount)
{
	if (!bCanRecieveDamage)
		return;

	if (HP <= 0)
		return;

	HP -= amount;
	IDamageable::Execute_BP_OnDamage(this, amount);
	
	if (HP <= 0)
	{
		Die();
	}
}

void AGauntletCharacter::Die()
{
	//Enable Ragdoll
	GetMesh()->SetSimulatePhysics(true);
	// schedule respawning
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AGauntletCharacter::Respawn, RespawnTime, false);
	//Disable Movement
	GetCharacterMovement()->DisableMovement();

	SetActorEnableCollision(false);

	//Broadcast Death Delegate
	OnPlayerDead.Broadcast();
}

void AGauntletCharacter::Respawn()
{
	Destroy();
}
