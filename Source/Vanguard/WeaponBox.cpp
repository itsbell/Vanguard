// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBox.h"
#include "Vanguard.h"
#include "BaseWeapon.h"
#include "VanguardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"

TArray<TWeakObjectPtr<AWeaponBox>> AWeaponBox::ActiveBoxes;

// Sets default values
AWeaponBox::AWeaponBox()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AWeaponBoxMesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetGenerateOverlapEvents(true);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarWidgetComponent->SetGenerateOverlapEvents(false);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(-30.f, 0.f, 120.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComponent->SetTwoSided(true);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.f, 30.f));
	HealthBarWidgetComponent->SetRelativeScale3D(FVector(0.3f));
	HealthBarWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();

	ActiveBoxes.Add(this);

	Health = MaxHealth;

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->InitWidget();
		HealthBarWidget = Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());
		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(1.f);
		}
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogVanguard, Warning, TEXT("PlayerPawn is nullptr."));
		return;
	}

	Character = Cast<AVanguardCharacter>(PlayerPawn);
	if (!Character)
	{
		UE_LOG(LogVanguard, Warning, TEXT("Character is nullptr."));
		return;
	}
}

void AWeaponBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// static이라 PIE 세션이 끝나도 배열이 남는다. 자기 자신과 무효해진 항목을 같이 정리한다.
	ActiveBoxes.RemoveAllSwap([this](const TWeakObjectPtr<AWeaponBox>& Box)
		{
			return !Box.IsValid() || Box.Get() == this;
		});

	Super::EndPlay(EndPlayReason);
}

void AWeaponBox::DestroyAll(const UWorld* World)
{
	// Destroy()가 EndPlay를 거쳐 ActiveBoxes를 수정하므로, 복사본을 순회한다
	TArray<TWeakObjectPtr<AWeaponBox>> BoxesToDestroy = ActiveBoxes;

	for (const TWeakObjectPtr<AWeaponBox>& BoxPtr : BoxesToDestroy)
	{
		AWeaponBox* Box = BoxPtr.Get();
		if (Box && Box->GetWorld() == World)
			Box->Destroy();
	}
}

void AWeaponBox::TakeDamageAmount(float DamageAmount)
{
	Health = FMath::Max(Health - DamageAmount, 0.f);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Health / MaxHealth);
	}

	if (Health <= 0.0f)
	{
		// 무기 획득. 스테이지 클리어의 DestroyAll과 구분되는 유일한 지점이라 여기서 지급한다
		if (Character && WeaponClass)
			Character->EquipWeapon(WeaponClass);

		Destroy();
	}
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Character)
		return;

	FVector ActorLocation = GetActorLocation();
	float CharacterY = Character->GetTargetLocation().Y;
	float DistanceY = FMath::Abs(ActorLocation.Y - CharacterY);

	float StopDistanceY = DistanceLimit; // 기본 정지선

	// 나보다 캐릭터에 가까운 박스가 있으면 그 뒤에 줄을 선다
	for (const TWeakObjectPtr<AWeaponBox>& BoxPtr : ActiveBoxes)
	{
		AWeaponBox* Other = BoxPtr.Get();
		if (!Other || Other == this)
			continue;

		if (Other->GetWorld() != GetWorld()) // static이라 PIE의 다른 월드 박스가 섞일 수 있다
			continue;

		float OtherDistanceY = FMath::Abs(Other->GetActorLocation().Y - CharacterY);
		if (OtherDistanceY < DistanceY)
			StopDistanceY = FMath::Max(StopDistanceY, OtherDistanceY + BoxSpacing);
	}

	if (DistanceY > StopDistanceY)
	{
		SetActorLocation(ActorLocation + GetActorForwardVector() * Speed * DeltaTime); // 직진
	}
}
