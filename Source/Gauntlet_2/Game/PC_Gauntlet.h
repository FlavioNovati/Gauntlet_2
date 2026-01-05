#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/Actors/Characters/GauntletCharacter.h"
#include "Gauntlet_2PlayerController.h"
#include "PC_Gauntlet.generated.h"

UCLASS()
class GAUNTLET_2_API APC_GauntletController : public AGauntlet_2PlayerController
{
	GENERATED_BODY()
	
protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	int UnlockedKeyAmount = 0;

	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable)
	void ImplementOnPlayerDead();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	TSubclassOf<AGauntletCharacter> CharacterClass;

	FTransform RespawnTransform;

	bool bIsInPause = false;

public:

	UFUNCTION()
	void RespawnPlayer(AActor* DestroyedActor);

};
