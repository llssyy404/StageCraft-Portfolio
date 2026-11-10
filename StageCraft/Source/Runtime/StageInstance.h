// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StageContext.h"
#include "StageInstance.generated.h"

class UStageDataAsset;
struct FStageStepData;
struct FStageObjective;
enum class EStageConditionResult : uint8;

UENUM()
enum class EStageState : uint8
{
	NotStarted,
	Ready,
	Playing,
	Failed,
	Completed
};

UENUM()
enum class EStageStepState : uint8
{
	None,
	ExecutingActions,
	WaitingConditions,
	Completed
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageObjectiveChanged, const FStageObjective&/*Objective*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageStateChanged, const EStageState/*StageState*/);


UCLASS()
class STAGECRAFT_API UStageInstance : public UObject
{
	GENERATED_BODY()
	
public:
	bool Initialize(UStageDataAsset* InStageDataAsset);
	void Prepare();
	void Start();
	void Complete();
	void Fail();
	void Update(float InDeltaTime);

	FName GetStageId() const;
	FStageObjective GetCurrentObjective() const;

	void CompleteCurrentStep();

	void NotifyEnemyDeath(FName InContentId);
	void NotifyPlayerDeath();

	EStageState GetStageState() const { return StageState; }
	int32 GetCurrentStepIndex() const { return CurrentStepIndex; }

private:
	void SetStageState(EStageState InStageState);

	void StartStep();
	void AdvanceStep();
	void TryCompleteStep();

	void ExecuteCurrentAction();
	void OnActionCompleted(bool bInSuccess);

	EStageConditionResult CheckConditions();

public:
	FOnStageObjectiveChanged OnObjectiveChanged;
	FOnStageStateChanged OnStageStateChanged;

private:
	FTimerHandle ReadyTimerHandle;
	EStageState StageState = EStageState::NotStarted;
	EStageStepState StageStepState = EStageStepState::None;
	int32 CurrentStepIndex = INDEX_NONE;
	int32 CurrentActionIndex = INDEX_NONE;

	const FStageStepData* StepData = nullptr;

	FStageContext StageContext;

	UPROPERTY()
	TObjectPtr<UStageDataAsset> StageDataAsset;
};
