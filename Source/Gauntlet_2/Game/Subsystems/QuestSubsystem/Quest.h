#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCallback, UQuest*, Quest);

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GAUNTLET_2_API UQuest : public UObject
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static UQuest* CreateQuest(FString questName, FString questDescription, UQuest* subQuest);

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FString QuestName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FString QuestDescription;

	AActor* ContextActor;
	TFunction<void()>* FunctionToExecute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	UQuest* SubQuest;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	bool bOneTimeQuest = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	int QuestIndex = 0;

	bool bIsCompleted;

public:
	UQuest();
	UQuest(FString questName, FString questDescription, TFunction<void()>* functionToExecute, UQuest* subQuest);

	UFUNCTION(BlueprintCallable)
	void EnableQuest(AActor* contextActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestEnabled(AActor* contextActor);

	UFUNCTION(BlueprintCallable)
	void DisableQuest(AActor* contextActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestDisabled(AActor* contextActor);

	UFUNCTION(BlueprintCallable)
	void CompleteQuest(AActor* contextActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestCompleted(AActor* contextActor, UWorld* worldReference);


	//Getters
	UFUNCTION(BlueprintCallable)
	FString GetQuestName() { return QuestName; }
	UFUNCTION(BlueprintCallable)
	FString GetQuestDescription() { return QuestDescription; }
	UFUNCTION(BlueprintCallable)
	UQuest* GetSubquest() { return SubQuest; }
	UFUNCTION(BlueprintCallable)
	bool IsCompleted() { return bIsCompleted; }
	UFUNCTION(BlueprintCallable)
	bool IsOneTime() { return bOneTimeQuest; }
	UFUNCTION(BlueprintCallable)
	int GetQuestIndex() { return QuestIndex; }

	//Callbacks
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FQuestCallback OnQuestCompletedCallback;
};
