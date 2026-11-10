// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Condition/StageCondition_KillTarget.h"


EStageConditionResult UStageCondition_KillTarget::IsSatisfied(const FStageContext& InContext)
{
	if (TargetContentId == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStageCondition_KillTarget::IsSatisfied, Invalid TargetContentId."));
		return EStageConditionResult::Invalid;
	}

	if (RequiredKillCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStageCondition_KillTarget::IsSatisfied, Invalid RequiredKillCount."));
		return EStageConditionResult::Invalid;
	}

	const int32* CurrentKillCount = InContext.EnemyDeathCounts.Find(TargetContentId);
	if (CurrentKillCount)
	{
		return *CurrentKillCount >= RequiredKillCount ? EStageConditionResult::Satisfied : EStageConditionResult::NotSatisfied;
	}

	return EStageConditionResult::NotSatisfied;
}