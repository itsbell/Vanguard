// Fill out your copyright notice in the Description page of Project Settings.


#include "StageManager.h"

// Sets default values
AStageManager::AStageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageManager::BeginPlay()
{
	Super::BeginPlay();
	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawner is not set in StageManager class."));
		return;
	}

	StartWave();
}

// Called every frame
void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageManager::StartWave()
{
	if (StageIndex >= Stages.Num()) return;
	if (WaveIndex >= Stages[StageIndex].Waves.Num()) return;

	FStage& Stage = Stages[StageIndex];
	FWave& Wave = Stage.Waves[WaveIndex];
	SpawnEntryStatuses.Init(FSpawnEntryStatus(), Wave.SpawnEntries.Num());
}

