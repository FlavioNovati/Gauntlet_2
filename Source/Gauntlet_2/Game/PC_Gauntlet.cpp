#include "Game/PC_Gauntlet.h"

void APC_GauntletController::BeginPlay()
{
	Super::BeginPlay();
}

void APC_GauntletController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APC_GauntletController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AGauntletCharacter* gauntletPlayer;
	gauntletPlayer = Cast<AGauntletCharacter>(aPawn);

	if (gauntletPlayer)
	{
		gauntletPlayer->OnPlayerDead.AddDynamic(this, &APC_GauntletController::ImplementOnPlayerDead);
	}

	aPawn->OnDestroyed.AddDynamic(this, &APC_GauntletController::RespawnPlayer);
	RespawnTransform = aPawn->GetTransform();
}

void APC_GauntletController::ImplementOnPlayerDead()
{

}

void APC_GauntletController::RespawnPlayer(AActor* DestroyedActor)
{
	if (!GetWorld())
		return;

	// spawn a new character at the respawn transform
	if (AGauntletCharacter* RespawnedCharacter = GetWorld()->SpawnActor<AGauntletCharacter>(CharacterClass, RespawnTransform))
	{
		// possess the character
		Possess(RespawnedCharacter);
	}
}