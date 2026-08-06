// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBox.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"

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
		// ¹«±â È¹µæ
		Destroy();
	}
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorLocation = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();

	SetActorLocation(ActorLocation + ActorForward * Speed * DeltaTime);
}

