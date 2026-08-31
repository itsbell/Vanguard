#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "VanguardCharacter.generated.h" // VanguardCharacter.generated.h" must be the last include in the header!!!

class ABaseWeapon;
class UInputAction;
class UInputComponent;
class UHealthBarWidget;
class UCameraComponent;
class AVanguardCharacter;
class USpringArmComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVanguardCharacterDied, AVanguardCharacter*, DeadCharacter);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AVanguardCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY()
	ABaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	float Health = 100.0f;

	const float BaseWalkSpeed = 500.f;

	bool bIsDead = false;

	// 전투(발사) 허용 상태. 게임 중 새로 장착한 무기의 틱을 이 값으로 맞춘다
	bool bCombatEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

public:

	/** Constructor */
	AVanguardCharacter();	

	void OnGameStarted();
    void OnGameEnded();

	// 기존 무기를 버리고 새로 장착. 무기 변경의 유일한 진입점
	void EquipWeapon(TSubclassOf<ABaseWeapon> NewWeaponClass);
	void TakeDamageAmount(float Damage);


	UPROPERTY(BlueprintAssignable)
	FOnVanguardCharacterDied OnCharacterDied;

protected:
	virtual void BeginPlay() override;

	void UpdateWalkSpeed(); // 현재 무기 Weight로 MaxWalkSpeed 갱신

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

