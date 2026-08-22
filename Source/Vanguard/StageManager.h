// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageManager.generated.h"


class AEnemy;
class AEnemySpawner;
class AVanguardCharacter;
class ABaseWeapon;
class AWeaponBox;
class UUserWidget;
class UStageBannerWidget;

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
	int32 Count = 10;

	UPROPERTY(EditAnywhere)
	float Interval = 1.0f;
};

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBox> WeaponBoxClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;
};

USTRUCT(BlueprintType)
struct FStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWave> Waves;
};

struct FSpawnEntryStatus
{
	float Timer = 0.0f;
	int32 Count = 0;
};

UCLASS()
class VANGUARD_API AStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

private:
	int32 StageIndex = 0;
	int32 WaveIndex = 0;
	int32 AliveEnemyCount = 0;
	int32 KillCount = 0;

	UPROPERTY(EditAnywhere, Category = "위젯")
	TSubclassOf<UUserWidget> IntroWidgetClass;

	UPROPERTY(EditAnywhere, Category = "위젯")
	TSubclassOf<UUserWidget> AllClearWidgetClass;

	UPROPERTY(EditAnywhere, Category = "위젯")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, Category = "위젯")
    TSubclassOf<UStageBannerWidget> StageBannerWidgetClass;

	UPROPERTY()
	UStageBannerWidget* BannerWidget;

	FTimerHandle TransitionTimerHandle;
	float BannerDuration = 1.5f;
	float TransitionGap = 0.5f;

	UPROPERTY(EditAnywhere)
	TArray<FStage> Stages;

	TArray<FSpawnEntryStatus> SpawnEntryStatuses;

	UPROPERTY(EditInstanceOnly)
	AEnemySpawner* Spawner;

	UPROPERTY()
	AVanguardCharacter* Character;

	void StartWave();

	UFUNCTION()
	void HandleEnemyDied(AEnemy* DeadEnemy);
	
	UFUNCTION()
	void HandleCharacterDied(AVanguardCharacter* DeadCharacter);

	void BeginStage();
	void StartSpawning();
	void FinishClear();
	void ShowEndScreen(TSubclassOf<UUserWidget> WidgetClass);
};