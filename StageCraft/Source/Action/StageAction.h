// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Stage/Runtime/StageContext.h"
#include "StageAction.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageActionCompleted, bool/*bSuccess*/);

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class STAGECRAFT_API UStageAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FStageContext& InContext)
		PURE_VIRTUAL(UStageAction::Execute, );

public:
	FOnStageActionCompleted OnCompleted;
};
