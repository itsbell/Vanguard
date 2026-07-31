// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "VanguardCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
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
}

void AEnemy::TakeDamageAmount(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		OnEnemyDied.Broadcast(this);
		Destroy();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is nullptr."));
		return;
	}
	AttackTimer += DeltaTime;

	FVector ActorLocation = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();
	FVector ActorRightForward = GetActorRightVector();
	FVector CharacterLocation = Character->GetTargetLocation();
	FVector MoveVector = CharacterLocation - ActorLocation;
	MoveVector.Z = 0;
	MoveVector.Normalize(); // 단위 벡터

	float Distance = GetDistanceTo(Character);
	float DistanceY = FMath::Abs(ActorLocation.Y - CharacterLocation.Y);

	if (DistanceY > DistanceStraight)
	{
		SetActorLocation(ActorLocation + ActorForward * Speed * DeltaTime); // 직진
	}
	else if (DistanceY > DistanceLimit)
	{
		if (Distance <= AttackRange)
		{
			if (AttackTimer > AttackSpeed)
			{
				Character->TakeDamageAmount(Damage);
				AttackTimer = 0;
			}
		}
		else
		{
			SetActorLocation(ActorLocation + MoveVector * Speed * DeltaTime); // 캐릭터가 있는 쪽으로 대각선 방향으로 추격
		}
	}
	else // 더이상 Y방향으로 다가갈 수 없는 상태
	{
		if (Distance <= AttackRange)
		{
			if (AttackTimer > AttackSpeed)
			{
				Character->TakeDamageAmount(Damage);
				AttackTimer = 0;
			}
		}
		else
		{
			SetActorLocation(ActorLocation + FVector(FMath::Sign(MoveVector.X),0,0) * Speed * DeltaTime); // 캐릭터가 있는 쪽으로 X 방향으로 추격
		}
	}
}

