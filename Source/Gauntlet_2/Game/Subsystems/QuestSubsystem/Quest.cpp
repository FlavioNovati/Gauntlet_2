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

void UQuest::EnableQuest()
{
	if (!GetWorld())
		return;

	UQuestSubsystem* questSubsystem = GetWorld()->GetSubsystem<UQuestSubsystem>();

	if (questSubsystem)
		questSubsystem->AddQuest(this);
}

void UQuest::DisableQuest()
{
	if (!GetWorld())
		return;

	UQuestSubsystem* questSubsystem = GetWorld()->GetSubsystem<UQuestSubsystem>();

	if (questSubsystem)
		questSubsystem->DisabeQuest(this);
}

void UQuest::CompleteQuest(AActor* contextActor)
{
	if (!GetWorld())
		return;

	ContextActor = contextActor;
	
	if (FunctionToExecute)
		(*FunctionToExecute)();

	UQuestSubsystem* questSubsystem = GetWorld()->GetSubsystem<UQuestSubsystem>();

	if (questSubsystem)
		questSubsystem->CompleteQuest(this, ContextActor);

	OnQuestCompletedCallback.Broadcast(this);
	OnQuestCompleted(contextActor, GetWorld());
}


