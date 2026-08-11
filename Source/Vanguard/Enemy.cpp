// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "VanguardCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "TimerManager.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 적끼리, 플레이어와 서로 밀지 않는다. 총알 오버랩은 그대로 유지된다
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// 마네퀸 계열 메시 기준 정렬. 발을 캡슐 바닥으로 내리고, 메시의 -Y 정면을 캡슐 정면(+X)에 맞춘다
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -89.f), FRotator(0.f, -90.f, 0.f));

	// 이 적은 컨트롤러 없이 Tick에서 스스로 이동 방향을 정한다.
	// CharacterMovement는 컨트롤러가 없으면 이동/회전 연산을 통째로 건너뛰므로 이 플래그가 필요하다.
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	bUseControllerRotationYaw = false;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarWidgetComponent->SetGenerateOverlapEvents(false);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComponent->SetTwoSided(true);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.f, 30.f));
	HealthBarWidgetComponent->SetRelativeScale3D(FVector(0.3f));
	HealthBarWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	LaneForward = GetActorForwardVector();
	GetCharacterMovement()->MaxWalkSpeed = Speed;

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->InitWidget();
		HealthBarWidget = Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());
		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(1.f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HealthBarWidget cast failed."));
		}
	}

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
	if (bIsDead)
		return;

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Health / MaxHealth);
	}

	if (Health <= 0.0f)
	{
		bIsDead = true;
		OnEnemyDied.Broadcast(this);

		SetActorTickEnabled(false); // 런타임 틱 일시 중지
		GetCharacterMovement()->StopMovementImmediately();	// 현재: 지금 움직이던 관성을 즉시 제거
		GetCharacterMovement()->DisableMovement();			// 미래: 이후의 이동 처리를 아예 차단
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (HealthBarWidgetComponent)
			HealthBarWidgetComponent->SetHiddenInGame(true);

		const float MontageLength = PlayAnimMontage(DeathMontage);
		if (MontageLength > 0.0f)
			GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy::OnDeathFinished, MontageLength, false);
		else
			OnDeathFinished();
	}
}

void AEnemy::OnDeathFinished()
{
	Destroy();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Character)
		return;

	AttackTimer += DeltaTime;

	FVector ActorLocation = GetActorLocation();
	FVector CharacterLocation = Character->GetTargetLocation();
	FVector MoveVector = CharacterLocation - ActorLocation;
	MoveVector.Z = 0;
	MoveVector.Normalize(); // 단위 벡터

	float Distance = GetDistanceTo(Character);
	float DistanceY = FMath::Abs(ActorLocation.Y - CharacterLocation.Y);

	if (DistanceY > DistanceStraight)
	{
		AddMovementInput(LaneForward); // 직진
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
			AddMovementInput(MoveVector); // 캐릭터가 있는 쪽으로 대각선 방향으로 추격
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
			AddMovementInput(FVector(FMath::Sign(MoveVector.X), 0.f, 0.f)); // 캐릭터가 있는 쪽으로 X 방향으로 추격
		}
	}
}
