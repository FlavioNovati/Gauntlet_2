#include "Game/PC_Gauntlet.h"
#include "Game/Interfaces/Pickable.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

void APC_GauntletController::BeginPlay()
{
	Super::BeginPlay();

	OnMainMenu();
}

void APC_GauntletController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &APC_GauntletController::Pause);
	}
}

//Game States -> Moved here to speed up the developing time they should be moved back in game instance

void APC_GauntletController::Pause()
 {
	if (bIsInMainMenu)
		return;

	bIsInPause = !bIsInPause;
	bIsInPause ? OnPause() : OnGameplay();
}

void APC_GauntletController::Play()
{
	bIsInMainMenu = false;
	bIsInPause = false;

	OnGameplay();
	OnPlay();
}

void APC_GauntletController::ReturnToMainMenu()
{
	bIsInMainMenu = true;
	bIsInPause = false;

	OnMainMenu();

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
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

void APC_GauntletController::ImplementOnPlayerDead(AGauntletCharacter* GauntletPlayer)
{
	TScriptInterface<IPickable> pickable = TScriptInterface<IPickable>(GauntletPlayer->GetPickedActor());

	if (pickable.GetObject() != nullptr)
	{
		pickable.GetInterface()->ResetPickable();
	}
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

void APC_GauntletController::WinGame()
{
	ReturnToMainMenu();
}
