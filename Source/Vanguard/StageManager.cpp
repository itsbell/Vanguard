// Fill out your copyright notice in the Description page of Project Settings.


#include "StageManager.h"
#include "EnemySpawner.h"
#include "Characters/Enemy.h"
#include "WeaponBox.h"
#include "Characters/VanguardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

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
	
	SetActorTickEnabled(false);
	
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


	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}

	if (IntroWidgetClass)
	{
		UUserWidget* IntroWidget = CreateWidget<UUserWidget>(GetWorld(), IntroWidgetClass);
		if (IntroWidget)
		{
			IntroWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create IntroWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IntroWidgetClass is not set"));
    }


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
			AWeaponBox::DestroyAll(GetWorld()); // StartWave가 다음 스테이지 박스를 스폰하기 전에 정리한다
			StageIndex++;
			WaveIndex = 0;
			StartWave();
		}
	}
}

void AStageManager::StartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
	
	if (Character)
	{
		Character->OnGameStarted();
	}

	SetActorTickEnabled(true);
	StartWave();
}

void AStageManager::StartWave()
{
	if (StageIndex >= Stages.Num()) return;
	if (WaveIndex >= Stages[StageIndex].Waves.Num()) return;

	FStage& Stage = Stages[StageIndex];
	FWave& Wave = Stage.Waves[WaveIndex];
	SpawnEntryStatuses.Init(FSpawnEntryStatus(), Wave.SpawnEntries.Num());
	
	if (Wave.WeaponBoxClass)
		Spawner->SpawnWeaponBox(Wave.WeaponBoxClass, Wave.WeaponClass);
}

void AStageManager::HandleEnemyDied(AEnemy* DeadEnemy)
{
	AliveEnemyCount--;
}

void AStageManager::HandleCharacterDied(AVanguardCharacter* DeadCharacter)
{
	SetActorTickEnabled(false);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}

	if (GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
		}
	}
	else
        UE_LOG(LogTemp, Warning, TEXT("GameOverWidgetClass is not set"));
}

