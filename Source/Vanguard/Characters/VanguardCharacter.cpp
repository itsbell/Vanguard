#include "VanguardCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Vanguard.h"
#include "Weapons/BaseWeapon.h"
#include "HealthBarWidget.h"

AVanguardCharacter::AVanguardCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AVanguardCharacter::OnGameStarted()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorTickEnabled(true);
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AVanguardCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	if (HealthBarWidgetClass)
	{
		HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->AddToViewport();
            HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
			HealthBarWidget->SetHealthPercent(1.f);
		}
	}

	if (!WeaponClass)
	{
		UE_LOG(LogVanguard, Error, TEXT("'%s' WeaponClass is not set! Please set a WeaponClass in the Blueprint or C++ class."), *GetNameSafe(this));
		return;
	}

	EquipWeapon(WeaponClass);
}

void AVanguardCharacter::EquipWeapon(TSubclassOf<ABaseWeapon> NewWeaponClass)
{
	if (!NewWeaponClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(NewWeaponClass, SpawnParams);
	if (!SpawnedWeapon)
	{
		UE_LOG(LogVanguard, Error, TEXT("Failed to spawn weapon '%s'."), *GetNameSafe(NewWeaponClass));
		return; // 스폰 실패 시 기존 무기를 그대로 유지한다
	}

	if (CurrentWeapon)
		CurrentWeapon->Destroy(); // 낡은 무기 소멸. 안 하면 계속 붙어서 발사된다

	CurrentWeapon = SpawnedWeapon;
	SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));

	UpdateWalkSpeed();
}

void AVanguardCharacter::UpdateWalkSpeed()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		const float WeaponWeight = CurrentWeapon ? CurrentWeapon->GetWeight() * 5 : 0.0f;
		MoveComp->MaxWalkSpeed = FMath::Clamp(BaseWalkSpeed - WeaponWeight, 100.f, BaseWalkSpeed);
	}
}

void AVanguardCharacter::TakeDamageAmount(float Damage)
{
	if (bIsDead) return;

	Health = FMath::Max(Health - Damage, 0.0f);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Health / MaxHealth);
	}

	if (Health <= 0.0f)
	{
		bIsDead = true;
		
		if (CurrentWeapon)
			CurrentWeapon->SetActorTickEnabled(false);

		OnCharacterDied.Broadcast(this);
	}
}

void AVanguardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVanguardCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AVanguardCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVanguardCharacter::Look);
	}
	else
	{
		UE_LOG(LogVanguard, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AVanguardCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AVanguardCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AVanguardCharacter::DoMove(float Right, float /*Forward*/ )
{
	const FVector RightDirection = FVector(1.0f, 0.0f, 0.0f);
	AddMovementInput(RightDirection, Right);
}

void AVanguardCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}