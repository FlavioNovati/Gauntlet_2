#include "Game/Subsystems/QuestSubsystem/QuestSubsystem.h"
#include "Game/Subsystems/QuestSubsystem/QuestFeedback.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"

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

		PlayQuestFeedback(quest, contextActor);
		LoadQuestFeedback(subquest);
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

	LoadQuestFeedback(quest);
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
	PlayQuestFeedback(quest, contextActor);

	OnEnabledQuestChanged.Broadcast(ActiveQuests);
	OnCompletedQuestChanged.Broadcast(CompletedQuests);
}

void UQuestSubsystem::LoadQuestFeedback(UQuest* quest)
{
	if (quest == nullptr)
		return;

	TObjectPtr<UQuestFeedback> feedback = quest->GetQuestFeedback();

	if (feedback == nullptr)
		return;

	TSoftObjectPtr<USoundBase> soundPtr = feedback->GetSoundPtr();
	TSoftObjectPtr<UParticleSystem> particlePtr = feedback->GetParticleSystemPtr();

	//Load Assets
	UAssetManager::GetStreamableManager().RequestAsyncLoad( soundPtr.ToSoftObjectPath() );
	UAssetManager::GetStreamableManager().RequestAsyncLoad( particlePtr.ToSoftObjectPath() );
}

void UQuestSubsystem::PlayQuestFeedback(UQuest* quest, AActor* contextActor)
{
	if (quest == nullptr)
		return;

	TObjectPtr<UQuestFeedback> feedback = quest->GetQuestFeedback();

	if (feedback == nullptr)
		return;

	TSoftObjectPtr<USoundBase> soundPtr = feedback->GetSoundPtr();
	TSoftObjectPtr<UParticleSystem> particlePtr = feedback->GetParticleSystemPtr();

	if (soundPtr != nullptr)
	{
		if (USoundBase* loadedSound = soundPtr.Get())
		{
			UGameplayStatics::PlaySound2D(GetWorld(), loadedSound);
		}
	}

	if (particlePtr != nullptr)
	{
		if (UParticleSystem* particleSys = particlePtr.Get())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleSys, contextActor->GetTransform(), true, EPSCPoolMethod::AutoRelease, true);
		}
	}
}
