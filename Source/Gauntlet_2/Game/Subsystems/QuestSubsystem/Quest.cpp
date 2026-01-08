#include "Game/Subsystems/QuestSubsystem/Quest.h"
#include "Game/Subsystems/QuestSubsystem/QuestSubsystem.h"

UQuest* UQuest::CreateQuest(FString questName, FString questDescription, UQuest* subQuest)
{
	UQuest* quest = NewObject<UQuest>();
	quest->QuestName = questName;
	quest->QuestDescription = questDescription;
	quest->SubQuest = subQuest;
	return quest;
}

UQuest::UQuest()
{

}

UQuest::UQuest(FString questName, FString questDescription, TFunction<void()>* InFunction, UQuest* InSubQuest)
{
	QuestName = questName;
	QuestDescription = questDescription;
	FunctionToExecute = MoveTemp(InFunction);
	SubQuest = InSubQuest;
}

void UQuest::EnableQuest(AActor* contextActor)
{
	OnQuestEnabled(contextActor);
}

void UQuest::DisableQuest(AActor* contextActor)
{
	OnQuestDisabled(contextActor);
}

void UQuest::CompleteQuest(AActor* contextActor)
{
	if (!GetWorld())
		return;

	ContextActor = contextActor;
	
	if (FunctionToExecute)
		(*FunctionToExecute)();

	OnQuestCompletedCallback.Broadcast(this);
	OnQuestCompleted(contextActor, GetWorld());
}


