// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StageStepData.h"
#include "StageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class STAGECRAFT_API UStageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FName GetStageId() const { return StageId; }
	float GetReadyDuration() const { return ReadyDuration; }
	FName GetNextStageId() const { return NextStageId; }
	int32 GetStepCount() const { return Steps.Num(); }
	const FStageStepData* GetStepAt(int32 Index) const { return Steps.IsValidIndex(Index) ? &Steps[Index] : nullptr; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	FName StageId;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stage", meta = (ClampMin = "0.01"))
	float ReadyDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	FName NextStageId;

	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	TArray<FStageStepData> Steps;
};
