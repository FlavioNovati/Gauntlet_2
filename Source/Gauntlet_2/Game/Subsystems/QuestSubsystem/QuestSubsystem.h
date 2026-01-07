#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Game/Subsystems/QuestSubsystem/Quest.h"
#include "QuestSubsystem.generated.h"

UCLASS()
class GAUNTLET_2_API UQuestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<UQuest*> InactiveQuests;
	TArray<UQuest*> ActiveQuests;
	TArray<UQuest*> CompletedQuests;

public:
	UFUNCTION(BlueprintCallable)
	void AddQuest(UQuest* quest);
	UFUNCTION(BlueprintCallable)
	void DisabeQuest(UQuest* quest);
	UFUNCTION(BlueprintCallable)
	void CompleteQuest(UQuest* quest, AActor* contextActor);
};
