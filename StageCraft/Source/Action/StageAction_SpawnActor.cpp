// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Action/StageAction_SpawnActor.h"

#include "Stage/Actor/StageEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void UStageAction_SpawnActor::Execute(const FStageContext& InContext)
{
	bool bSuccess = true;
	if (!IsValid(InContext.World))
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_SpawnActor::Execute, Invalid World !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	if (!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UStageAction_SpawnActor::Execute, Invalid ActorClass !!"));
		OnCompleted.Broadcast(false);
		return;
	}

	if (ActorClass->IsChildOf(AStageEnemyCharacter::StaticClass()))
	{
		if (SpawnContentId == NAME_None)
		{
			UE_LOG(LogTemp, Warning, TEXT("UStageAction_SpawnActor::Execute, Invalid SpawnContentId"));
			OnCompleted.Broadcast(false);
			return;
		}
	}

	FActorSpawnParameters SpawnParams;
	FTransform NewSpawnTransform;
	if (SpawnMode == EStageSpawnMode::Normal)
	{
		NewSpawnTransform = SpawnTransform;
	}
	else if(SpawnMode == EStageSpawnMode::TargetPositionSpawn)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(InContext.World, AActor::StaticClass(), SpawnLocationTargetTag, FoundActors);
		if (FoundActors.Num() == 0 || !IsValid(FoundActors[0]))
		{
			UE_LOG(LogTemp, Warning, TEXT("UStageAction_SpawnActor::Execute, Invalid SpawnLocationTargetTag"));
			OnCompleted.Broadcast(false);
			return;
		}

		NewSpawnTransform = FoundActors[0]->GetActorTransform();
		NewSpawnTransform.AddToTranslation(SpawnLocationOffset);
		NewSpawnTransform.SetScale3D(FVector::OneVector);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UStageAction_SpawnActor::Execute, Invalid SpawnMode"));
		OnCompleted.Broadcast(false);
		return;
	}

	AActor* SpawnedActor = InContext.World->SpawnActor<AActor>(ActorClass, NewSpawnTransform, SpawnParams);
	if (IsValid(SpawnedActor))
	{
		if (SpawnActorTag != NAME_None)
		{
			SpawnedActor->Tags.AddUnique(SpawnActorTag);
		}

		AStageEnemyCharacter* EnemyCharacter = Cast<AStageEnemyCharacter>(SpawnedActor);
		if (IsValid(EnemyCharacter))
		{
			EnemyCharacter->SetContentId(SpawnContentId);
		}

		UE_LOG(LogTemp, Warning, TEXT("Actor Spawned : %s"), *SpawnedActor->GetName());
	}
	else
	{
		bSuccess = false;
		UE_LOG(LogTemp, Error, TEXT("UStageAction_SpawnActor::Execute, Actor Spawn Failed."));
	}

	OnCompleted.Broadcast(bSuccess);
}
