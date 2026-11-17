
#include "Stage/Action/StageAction_PlaySequence.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"


void UStageAction_PlaySequence::Execute(const FStageContext& InContext)
{
	if (!IsValid(InContext.World))
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_PlaySequence::Execute, Invalid World !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	if (!LevelSequence)
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_PlaySequence::Execute, Invalid LevelSequence !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ALevelSequenceActor* CreatedActor = nullptr;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(InContext.World, LevelSequence, PlaybackSettings, CreatedActor);
	if (!IsValid(SequencePlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_PlaySequence::Execute, Failed to create SequencePlayer !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	SequenceActor = CreatedActor;

	SequencePlayer->OnFinished.AddDynamic(this, &UStageAction_PlaySequence::OnSequenceFinished);
	SequencePlayer->Play();
}

void UStageAction_PlaySequence::Release()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnFinished.RemoveDynamic(this, &UStageAction_PlaySequence::OnSequenceFinished);
	}
	SequencePlayer = nullptr;

	if (IsValid(SequenceActor))
	{
		SequenceActor->Destroy();
	}
	SequenceActor = nullptr;
}

void UStageAction_PlaySequence::Abort()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnFinished.RemoveDynamic(this, &UStageAction_PlaySequence::OnSequenceFinished);
		SequencePlayer->Stop();
	}
}

void UStageAction_PlaySequence::OnSequenceFinished()
{
	OnCompleted.Broadcast(true);
}