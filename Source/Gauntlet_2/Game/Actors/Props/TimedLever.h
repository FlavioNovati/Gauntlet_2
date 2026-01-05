#pragma once

#include "CoreMinimal.h"
#include "Game/Actors/Props/Lever.h"
#include "TimedLever.generated.h"

UCLASS()
class GAUNTLET_2_API ATimedLever : public ALever
{
	GENERATED_BODY()
	
	FTimerHandle FlickLeverTimerHandle;
	bool bCanInteract = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess))
	float ResetFlickTime = 15.f;
	
	void ResetLever();

public:
	bool NativeCanInteract(AActor* contextActor) override;

	void NativeInteract(AActor* contextActor) override;
};
