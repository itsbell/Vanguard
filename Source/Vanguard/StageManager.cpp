// Fill out your copyright notice in the Description page of Project Settings.


#include "StageManager.h"
#include "EnemySpawner.h"
#include "Enemy.h"
#include "WeaponBox.h"
#include "VanguardCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is nullptr."));
		return;
	}

	Character = Cast<AVanguardCharacter>(PlayerPawn);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is nullptr."));
		return;
	}
	Character->OnCharacterDied.AddDynamic(this, &AStageManager::HandleCharacterDied);

	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawner is not set"));
		return;
	}

	StartWave();
}

// Called every frame
void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StageIndex >= Stages.Num()) return;
	FStage& Stage = Stages[StageIndex];

	if (WaveIndex < Stage.Waves.Num())
	{
		FWave& Wave = Stage.Waves[WaveIndex];

		bool bWaveEnd = true;
		for (int32 i = 0; i < Wave.SpawnEntries.Num(); i++)
		{
			FSpawnEntry& SpawnEntry = Wave.SpawnEntries[i];
			FSpawnEntryStatus& SpawnEntryStatus = SpawnEntryStatuses[i];

			if (SpawnEntryStatus.Count < SpawnEntry.Count)
			{
				SpawnEntryStatus.Timer += DeltaTime;
				if (SpawnEntryStatus.Timer >= SpawnEntry.Interval)
				{
					AEnemy* SpawnedEnemy = Spawner->SpawnEnemy(SpawnEntry.EnemyClass);
					if (SpawnedEnemy)
					{
						SpawnedEnemy->OnEnemyDied.AddDynamic(this, &AStageManager::HandleEnemyDied);
						AliveEnemyCount++;
					}
					SpawnEntryStatus.Timer -= SpawnEntry.Interval;
					SpawnEntryStatus.Count++;
				}
				bWaveEnd = false;
			}
		}

		if (bWaveEnd)
		{
			WaveIndex++;
			StartWave();
		}
	}
	else
	{
		if (AliveEnemyCount == 0) // Stage Clear
		{
			StageIndex++;
			WaveIndex = 0;
			StartWave();
		}
	}
}

void AStageManager::StartWave()
{
	if (StageIndex >= Stages.Num()) return;
	if (WaveIndex >= Stages[StageIndex].Waves.Num()) return;

	FStage& Stage = Stages[StageIndex];
	FWave& Wave = Stage.Waves[WaveIndex];
	SpawnEntryStatuses.Init(FSpawnEntryStatus(), Wave.SpawnEntries.Num());
	
	if (Wave.WeaponBoxClass)
		Spawner->SpawnWeaponBox(Wave.WeaponBoxClass);
}

void AStageManager::HandleEnemyDied(AEnemy* DeadEnemy)
{
	AliveEnemyCount--;
}

void AStageManager::HandleCharacterDied(AVanguardCharacter* DeadCharacter)
{

}

