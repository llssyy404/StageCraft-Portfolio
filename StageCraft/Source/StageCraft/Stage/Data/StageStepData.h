// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageObjective.h"
#include "StageStepData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStageStepData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Stage")
	FName StepId;

	UPROPERTY(EditAnywhere, Category = "Stage")
	FStageObjective Objective;

	UPROPERTY(EditAnywhere, Instanced, Category = "Stage")
	TArray<TObjectPtr<class UStageAction>> Actions;

	UPROPERTY(EditAnywhere, Instanced, Category = "Stage")
	TArray<TObjectPtr<class UStageCondition>> Conditions;
};
