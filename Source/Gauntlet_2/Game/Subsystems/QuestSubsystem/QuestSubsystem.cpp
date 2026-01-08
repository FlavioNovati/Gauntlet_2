#include "Game/Subsystems/QuestSubsystem/QuestSubsystem.h"

void UQuestSubsystem::AddQuest(UQuest* quest)
{
	if (!quest)
		return;

	if(!InactiveQuests.Contains(quest))
		InactiveQuests.AddUnique(quest);

	OnQuestAdded.Broadcast(InactiveQuests);
}

void UQuestSubsystem::ProceedQuest(UQuest* quest, AActor* contextActor)
{
	if (!quest)
		return;

	if (!ActiveQuests.Contains(quest))
		return;

	UQuest* subquest = quest->GetSubquest();
	if (subquest)
	{
		ActiveQuests.Remove(quest);
		ActiveQuests.AddUnique(subquest);
		OnEnabledQuestChanged.Broadcast(ActiveQuests);
		return;
	}

	//No subquest -> complete quest
	CompleteQuest(quest, contextActor);
}

void UQuestSubsystem::RemoveQuest(UQuest* quest, AActor* contextActor)
{
	if (!quest)
		return;

	if (ActiveQuests.Contains(quest))
	{
		ActiveQuests.Remove(quest);
		OnEnabledQuestChanged.Broadcast(ActiveQuests);
		return;
	}

	if (InactiveQuests.Contains(quest))
	{
		InactiveQuests.Remove(quest);
		OnDisabledQuestChanged.Broadcast(InactiveQuests);
		return;
	}

	if (CompletedQuests.Contains(quest))
	{
		CompletedQuests.Remove(quest);
		OnCompletedQuestChanged.Broadcast(CompletedQuests);
		return;
	}
}

void UQuestSubsystem::EnableQuest(UQuest* quest, AActor* contextActor)
{
	if (!quest)
		return;

	if(!InactiveQuests.Contains(quest))
		return;
	
	InactiveQuests.Remove(quest);
	ActiveQuests.AddUnique(quest);
	quest->EnableQuest(contextActor);

	OnEnabledQuestChanged.Broadcast(ActiveQuests);
}

void UQuestSubsystem::DisableQuest(UQuest* quest, AActor* contextActor)
{
	if(!quest)
		return;

	if (!ActiveQuests.Contains(quest))
		return;
	
	if (CompletedQuests.Contains(quest) && !quest->IsOneTime())
		return;

	ActiveQuests.Remove(quest);
	InactiveQuests.Add(quest);

	quest->DisableQuest(contextActor);

	OnEnabledQuestChanged.Broadcast(ActiveQuests);
	OnDisabledQuestChanged.Broadcast(InactiveQuests);
}

void UQuestSubsystem::CompleteQuest(UQuest* quest, AActor* contextActor)
{
	if (!quest)
		return;

	if (!ActiveQuests.Contains(quest))
		return;

	ActiveQuests.Remove(quest);
	CompletedQuests.AddUnique(quest);

	quest->CompleteQuest(contextActor);

	OnEnabledQuestChanged.Broadcast(ActiveQuests);
	OnCompletedQuestChanged.Broadcast(CompletedQuests);
}
