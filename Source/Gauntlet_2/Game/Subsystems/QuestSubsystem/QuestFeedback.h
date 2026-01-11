#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestFeedback.generated.h"

UCLASS(BlueprintType)
class GAUNTLET_2_API UQuestFeedback : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> SoundToPlay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UParticleSystem> ParticlesToPlay;


public:
	TSoftObjectPtr<USoundBase> GetSoundPtr() { return SoundToPlay; }
	TSoftObjectPtr<UParticleSystem> GetParticleSystemPtr() { return ParticlesToPlay; }

};
