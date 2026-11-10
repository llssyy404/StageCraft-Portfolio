// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage/Action/StageAction.h"
#include "StageAction_PlaySequence.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

UCLASS()
class STAGECRAFT_API UStageAction_PlaySequence : public UStageAction
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FStageContext& InContext) override;

private:
	UFUNCTION()
	void OnSequenceFinished();

private:
	UPROPERTY(EditAnywhere, Category = "Sequence")
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY(Transient)
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
