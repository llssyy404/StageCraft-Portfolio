// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Action/StageAction_PlaySequence.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"


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
	ALevelSequenceActor* SequenceActor = nullptr;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(InContext.World, LevelSequence, PlaybackSettings, SequenceActor);
	if (!IsValid(SequencePlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_PlaySequence::Execute, Failed to create SequencePlayer !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	SequencePlayer->OnFinished.AddDynamic(this, &UStageAction_PlaySequence::OnSequenceFinished);
	SequencePlayer->Play();
}

void UStageAction_PlaySequence::OnSequenceFinished()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnFinished.RemoveDynamic(this, &UStageAction_PlaySequence::OnSequenceFinished);
	}

	OnCompleted.Broadcast(true);
}