// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage/Condition/StageCondition.h"
#include "StageCondition_KillTarget.generated.h"

/**
 * 
 */
UCLASS()
class STAGECRAFT_API UStageCondition_KillTarget : public UStageCondition
{
	GENERATED_BODY()
	
public:
	virtual EStageConditionResult IsSatisfied(const FStageContext& InContext) override;

private:
	UPROPERTY(EditAnywhere, Category = "Condition")
	FName TargetContentId;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (ClampMin = "1"))
	int32 RequiredKillCount;
};
