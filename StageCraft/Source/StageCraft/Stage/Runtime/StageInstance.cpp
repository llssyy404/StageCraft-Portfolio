// Fill out your copyright notice in the Description page of Project Settings.


#include "StageInstance.h"
#include "Stage/Data/StageDataAsset.h"
#include "Stage/Action/StageAction.h"
#include "Stage/Condition/StageCondition.h"

bool UStageInstance::Initialize(UStageDataAsset* InStageDataAsset)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize failed: World is invalid."));
		return false;
	}

	if (!IsValid(InStageDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize failed: StageDataAsset is invalid."));
		return false;
	}

	if (IsValid(StageDataAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("Initialize failed: StageInstance is already initialized."));
		return false;
	}

	if (InStageDataAsset->GetStepCount() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize failed: Stage has no steps."));
		return false;
	}

	StageDataAsset = InStageDataAsset;
	StageContext.World = World;
	
	return true;
}

void UStageInstance::Prepare()
{
	if (!IsValid(StageDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid StageDataAsset"));
		return;
	}

	if (StageState != EStageState::NotStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid StageState. CurrentState : %s"), *UEnum::GetValueAsString(StageState));
		return;
	}

	SetStageState(EStageState::Ready);

	GetWorld()->GetTimerManager().SetTimer(ReadyTimerHandle, this, &UStageInstance::Start, StageDataAsset->GetReadyDuration());
}

void UStageInstance::Start()
{
	if (!IsValid(StageDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid StageDataAsset"));
		return;
	}

	if (StageState != EStageState::Ready)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid StageState. CurrentState : %s"), *UEnum::GetValueAsString(StageState));
		return;
	}

	SetStageState(EStageState::Playing);

	CurrentStepIndex = 0;
	StartStep();
}

void UStageInstance::Complete()
{
	if (StageState != EStageState::Playing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid StageState. CurrentState : %s"), *UEnum::GetValueAsString(StageState));
		return;
	}

	SetStageState(EStageState::Completed);
}

void UStageInstance::Fail()
{
	if (StageState != EStageState::Playing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid StageState. CurrentState : %s"), *UEnum::GetValueAsString(StageState));
		return;
	}

	SetStageState(EStageState::Failed);
}

void UStageInstance::Update(float InDeltaTime)
{
	if (StageState != EStageState::Playing)
	{
		return;
	}

	if (StageStepState == EStageStepState::WaitingConditions)
	{
		TryCompleteStep();
	}
}

FName UStageInstance::GetStageId() const
{
	if (StageDataAsset == nullptr)
	{
		return FName();
	}

	return StageDataAsset->GetStageId();
}

FStageObjective UStageInstance::GetCurrentObjective() const
{
	if (StepData == nullptr)
	{
		return FStageObjective();
	}

	return StepData->Objective;
}

void UStageInstance::CompleteCurrentStep()
{
	if (StageState != EStageState::Playing)
	{
		UE_LOG(LogTemp, Warning, TEXT("CompleteCurrentStep failed: Stage is not Playing."));
		return;
	}

	if (StepData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CompleteCurrentStep failed: Invalid Current Step."));
		return;
	}

	StageStepState = EStageStepState::Completed;
	AdvanceStep();
}

void UStageInstance::NotifyEnemyDeath(FName InContentId)
{
	if (InContentId == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyEnemyDeath failed: ContentId is None."));
		return;
	}

	int32& KillCount = StageContext.EnemyDeathCounts.FindOrAdd(InContentId);
	++KillCount;

	UE_LOG(LogTemp, Warning, TEXT("UStageInstance::NotifyEnemyDeath ContentId: %s, KillCount: %d"), *InContentId.ToString(), KillCount);
}

void UStageInstance::NotifyPlayerDeath()
{
	Fail();
}

void UStageInstance::SetStageState(EStageState InStageState)
{
	StageState = InStageState;
	UE_LOG(LogTemp, Warning, TEXT("StageState : %s"), *UEnum::GetValueAsString(StageState));

	OnStageStateChanged.Broadcast(InStageState);
}

void UStageInstance::StartStep()
{
	CurrentActionIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("CurrentStepIndex : %d"), CurrentStepIndex);
	StepData = StageDataAsset->GetStepAt(CurrentStepIndex);
	if (StepData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Step Index : %d"), CurrentStepIndex);
		Fail();
		return;
	}

	OnObjectiveChanged.Broadcast(StepData->Objective);

	if (StepData->Actions.Num() > 0)
	{
		ExecuteCurrentAction();
	}
	else
	{
		TryCompleteStep();
	}
}

void UStageInstance::AdvanceStep()
{
	if (CurrentStepIndex + 1 < StageDataAsset->GetStepCount())
	{
		++CurrentStepIndex;
		StartStep();
	}
	else
	{
		Complete();
	}
}

void UStageInstance::TryCompleteStep()
{
	const EStageConditionResult ConditionResult = CheckConditions();

	switch (ConditionResult)
	{
	case EStageConditionResult::Satisfied:
	{
		StageStepState = EStageStepState::Completed;
		AdvanceStep();
	}
	break;
	case EStageConditionResult::NotSatisfied:
		StageStepState = EStageStepState::WaitingConditions;
		break;
	case EStageConditionResult::Invalid:
		Fail();
		break;
	}
}

void UStageInstance::ExecuteCurrentAction()
{
	if (StepData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Step Index : %d"), CurrentStepIndex);
		Fail();
		return;
	}

	StageStepState = EStageStepState::ExecutingActions;
	if (StepData->Actions.Num() <= CurrentActionIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("All Actions Completed"));
		TryCompleteStep();
		return;
	}

	UStageAction* Action = StepData->Actions.IsValidIndex(CurrentActionIndex) ? StepData->Actions[CurrentActionIndex] : nullptr;
	if (!IsValid(Action))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid StageAction, CurrentActionIndex : %d"), CurrentActionIndex);
		Fail();
		return;
	}

	Action->OnCompleted.Clear();
	Action->OnCompleted.AddUObject(this, &UStageInstance::OnActionCompleted);
	Action->Execute(StageContext);
}

void UStageInstance::OnActionCompleted(bool bInSuccess)
{
	if (bInSuccess)
	{
		++CurrentActionIndex;
		ExecuteCurrentAction();
	}
	else
	{
		Fail();
	}
}

EStageConditionResult UStageInstance::CheckConditions()
{
	if (StepData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Step Index : %d"), CurrentStepIndex);
		return EStageConditionResult::Invalid;
	}
	
	// Conditions are intentionally re-evaluated from the beginning
	// because satisfaction may change between ticks.
	for(int32 Index = 0; Index < StepData->Conditions.Num(); ++Index)
	{
		UStageCondition* Condition = StepData->Conditions.IsValidIndex(Index) ? StepData->Conditions[Index] : nullptr;
		if (!IsValid(Condition))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid StageCondition in CurrentStepIndex : %d, Condition Index: %d"), CurrentStepIndex, Index);
			return EStageConditionResult::Invalid;
		}

		EStageConditionResult ConditionResult = Condition->IsSatisfied(StageContext);
		if (ConditionResult != EStageConditionResult::Satisfied)
		{
			return ConditionResult;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("All Conditions Satisfied"));

	return EStageConditionResult::Satisfied;
}
