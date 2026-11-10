// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageInstance.h"
#include "Stage/Data/StageCollectionDataAsset.h"
#include "StageFlowSubsystem.generated.h"

class UStageSubsystem;

UCLASS(Config = Game)
class STAGECRAFT_API UStageFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void BindStageSubsystem(UStageSubsystem* InStageSubsystem);
	void RequestNextStage();

	const FStageCollectionEntry* FindNextStageEntry() const;
	bool CanMoveToNextStage() const;
	UStageDataAsset* GetCurrentStageData() const;

	bool ConsumePendingGold(int32& OutGold);

private:
	UPROPERTY(Config)
	FSoftObjectPath StageCollectionPath;

	UPROPERTY(Transient)
	TObjectPtr<UStageCollectionDataAsset> StageCollection = nullptr;

	TWeakObjectPtr<UStageSubsystem> BoundStageSubsystem;

	FName CurrentStageId;
	int32 PendingGold = -1;
};
