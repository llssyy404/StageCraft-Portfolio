// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Runtime/StageFlowSubsystem.h"

#include "Stage/Data/StageCollectionDataAsset.h"
#include "Stage/Data/StageDataAsset.h"
#include "Stage/Actor/StagePlayerCharacter.h"

#include "StageSubsystem.h"
#include "StageInstance.h"
#include "Kismet/GameplayStatics.h"

void UStageFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	StageCollection = Cast<UStageCollectionDataAsset>(StageCollectionPath.TryLoad());
	if (!IsValid(StageCollection))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StageCollection: %s"), *StageCollectionPath.ToString());
	}
}

void UStageFlowSubsystem::BindStageSubsystem(UStageSubsystem* InStageSubsystem)
{
	BoundStageSubsystem = IsValid(InStageSubsystem) ? InStageSubsystem : nullptr;
}

void UStageFlowSubsystem::RequestNextStage()
{
	const FStageCollectionEntry* NextStageEntry = FindNextStageEntry();
	if (NextStageEntry == nullptr)
	{
		return;
	}

	PendingGold = -1;
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AStagePlayerCharacter* PlayerCharacter = Cast<AStagePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (IsValid(PlayerCharacter))
		{
			PendingGold = PlayerCharacter->GetGold();
		}
	}
	
	if (PendingGold < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to capture player Gold during Stage transition."));
	}

	CurrentStageId = NextStageEntry->StageData->GetStageId();

	const FName LevelPackageName(*NextStageEntry->Level.ToSoftObjectPath().GetLongPackageName());
	UGameplayStatics::OpenLevel(this, LevelPackageName);
}

const FStageCollectionEntry* UStageFlowSubsystem::FindNextStageEntry() const
{
	UStageSubsystem* StageSubsystem = BoundStageSubsystem.Get();
	if (!IsValid(StageSubsystem) || !IsValid(StageCollection))
	{
		return nullptr;
	}

	UStageInstance* ActiveStage = StageSubsystem->GetActiveStage();
	if (!IsValid(ActiveStage) || ActiveStage->GetStageState() != EStageState::Completed)
	{
		return nullptr;
	}

	FName StageId = ActiveStage->GetStageId();
	const FStageCollectionEntry* CurrentStageEntry = StageCollection->FindStage(StageId);
	if (CurrentStageEntry == nullptr)
	{
		return nullptr;
	}

	UStageDataAsset* StageDataAsset = CurrentStageEntry->StageData;
	if (!IsValid(StageDataAsset))
	{
		return nullptr;
	}

	FName NextStageId = StageDataAsset->GetNextStageId();
	if (NextStageId.IsNone())
	{
		return nullptr;
	}

	const FStageCollectionEntry* NextStageEntry = StageCollection->FindStage(NextStageId);
	if (NextStageEntry == nullptr || !IsValid(NextStageEntry->StageData) || NextStageEntry->Level.IsNull())
	{
		return nullptr;
	}

	return NextStageEntry;
}

bool UStageFlowSubsystem::CanMoveToNextStage() const
{
	return FindNextStageEntry() != nullptr;
}

UStageDataAsset* UStageFlowSubsystem::GetCurrentStageData() const
{
	if (!IsValid(StageCollection))
	{
		return nullptr;
	}

	if (CurrentStageId.IsNone())
	{
		if(const FStageCollectionEntry* FirstStageEntry = StageCollection->GetFirstStage())
		{
			return FirstStageEntry->StageData;
		}
	}
	else
	{
		if (const FStageCollectionEntry* CurrentStageEntry = StageCollection->FindStage(CurrentStageId))
		{
			return CurrentStageEntry->StageData;
		}
	}

	return nullptr;
}

bool UStageFlowSubsystem::ConsumePendingGold(int32& OutGold)
{
	if (PendingGold < 0)
	{
		return false;
	}

	OutGold = PendingGold;
	PendingGold = -1;

	return true;
}
