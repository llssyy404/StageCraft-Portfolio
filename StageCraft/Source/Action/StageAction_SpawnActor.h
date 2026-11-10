// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage/Action/StageAction.h"
#include "StageAction_SpawnActor.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EStageSpawnMode : uint8
{
	Normal,
	TargetPositionSpawn
};

UCLASS()
class STAGECRAFT_API UStageAction_SpawnActor : public UStageAction
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FStageContext& InContext) override;

private:
	UPROPERTY(EditAnywhere, Category = "Stage")
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditAnywhere, Category = "Stage|Spawn")
	EStageSpawnMode SpawnMode = EStageSpawnMode::Normal;

	UPROPERTY(EditAnywhere, Category = "Stage|Spawn"
		, meta = (EditCondition = "SpawnMode == EStageSpawnMode::Normal", EditConditionHides))
	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, Category = "Stage|Spawn"
		, meta = (EditCondition = "SpawnMode == EStageSpawnMode::TargetPositionSpawn", EditConditionHides))
	FName SpawnLocationTargetTag;

	UPROPERTY(EditAnywhere, Category = "Stage|Spawn"
		, meta = (EditCondition = "SpawnMode == EStageSpawnMode::TargetPositionSpawn", EditConditionHides))
	FVector SpawnLocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Stage")
	FName SpawnActorTag;

	UPROPERTY(EditAnywhere, Category = "Stage")
	FName SpawnContentId;
};
