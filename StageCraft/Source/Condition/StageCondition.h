// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Stage/Runtime/StageContext.h"
#include "StageCondition.generated.h"

UENUM()
enum class EStageConditionResult : uint8
{
	Satisfied,
	NotSatisfied,
	Invalid
};

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class STAGECRAFT_API UStageCondition : public UObject
{
	GENERATED_BODY()
	
public:
	virtual EStageConditionResult IsSatisfied(const FStageContext& InContext)
		PURE_VIRTUAL(UStageCondition::IsSatisfied, return EStageConditionResult::NotSatisfied;);
};
