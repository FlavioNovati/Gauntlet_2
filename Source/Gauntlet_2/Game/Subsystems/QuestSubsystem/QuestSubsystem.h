#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Game/Subsystems/QuestSubsystem/Quest.h"
#include "QuestSubsystem.generated.h"

//This cannot be connected BP side
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestSubsystemCallback, const TArray<UQuest*>&, Quests);

UCLASS()
class GAUNTLET_2_API UQuestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<UQuest*> InactiveQuests;
	UPROPERTY()
	TArray<UQuest*> ActiveQuests;
	UPROPERTY()
	TArray<UQuest*> CompletedQuests;

public:
	UFUNCTION(BlueprintCallable)
	void AddQuest(UQuest* quest);

	UFUNCTION(BlueprintCallable)
	void ProceedQuest(UQuest* quest, AActor* contextActor);

	UFUNCTION(BlueprintCallable)
	void RemoveQuest(UQuest* quest, AActor* contextActor);

	UFUNCTION(BlueprintCallable)
	void EnableQuest(UQuest* quest, AActor* contextActor);
	UFUNCTION(BlueprintCallable)
	void DisableQuest(UQuest* quest, AActor* contextActor);
	UFUNCTION(BlueprintCallable)
	void CompleteQuest(UQuest* quest, AActor* contextActor);

	UFUNCTION(BlueprintCallable)
	const TArray<UQuest*> GetActiveQuests() { return ActiveQuests; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FQuestSubsystemCallback OnQuestAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FQuestSubsystemCallback OnEnabledQuestChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FQuestSubsystemCallback OnDisabledQuestChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FQuestSubsystemCallback OnCompletedQuestChanged;
};
