// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPSSurvivalCharacter.generated.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFPSSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()
	//ref to weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WEAPON", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> SK_Rifle;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
public:
	AFPSSurvivalCharacter();
	void BeginPlay();
	void EquipWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	// Death state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsDead;

	// Virtual function for character-specific setup
	virtual void SetupCharacter();

	// Animation Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* PlayMontage;
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

	// OOP: Polymorphic behavior (override)
	virtual void SetupCharacter() override; //working in project. IDK why its giving an error here

	//BP Call for TakeDamage section with custom event
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamage(float DamageAmount);

	// Weapon Handling
	void EquipWeapon();
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BulletDistance;

	UPROPERTY(EditAnywhere, Category = "Ammunition")
	int32 CurrentAmmunition;

	UPROPERTY(EditAnywhere, Category = "Ammunition")
	int32 TotalAmmunition;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraLocation;

	//UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<AActor> SK_Rifle;
};

