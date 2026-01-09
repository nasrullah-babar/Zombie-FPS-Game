// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSSurvivalCharacter.h"
#include "FPSSurvivalProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFPSSurvivalCharacter

AFPSSurvivalCharacter::AFPSSurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT(""));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT(""));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	// Variable initialisation
	FireDelay = 0.2f;
	BulletDistance = 10000.0f;
	CurrentAmmunition = 30;
	TotalAmmunition = 90;
	CameraLocation = FVector::ZeroVector;
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSSurvivalCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFPSSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	EquipWeapon();
}

void AFPSSurvivalCharacter::SetupCharacter()
{
	// Custom setup logic for this class
	UE_LOG(LogTemp, Warning, TEXT(""));
}

void AFPSSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSSurvivalCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AFPSSurvivalCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AFPSSurvivalCharacter::EquipWeapon()
{
	APlayerController* pController = Cast<APlayerController>(GetController());
	if (!pController || !SK_Rifle) return;

	const FRotator SpawnRot = pController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLoc = GetActorLocation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(SK_Rifle, SpawnLoc, SpawnRot, Params);
	if (SpawnedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT(""));
	}
}

void AFPSSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSSurvivalCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSSurvivalCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT(""), *GetNameSafe(this));
	}
}


void AFPSSurvivalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSSurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSSurvivalCharacter::EquipWeapon() {
	APlayerController* pController = Cast<APlayerController>(GetController());
	const FRotator pRotation = pController->PlayerCameraManager->GetCameraRotation();
	const FVector pLocation = GetOwner()->GetActorLocation();
	FActorSpawnParameters pSpawnParams;
	pSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* pPistol = GetWorld()->SpawnActor<AActor>(SK_Rifle, pLocation, pRotation, pSpawnParams);
}

void AFPSSurvivalCharacter::BeginPlay(){
	Super::BeginPlay();
	EquipWeapon();

}

AFPSSurvivalCharacter::AFPSSurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	RunningSpeed = 600.f;
	WalkSpeed = 300.f;
	CrouchSpeed = 150.f;
	Health = 100.f;
	bIsDead = false;
}

void AFPSSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupCharacter();
}

void AFPSSurvivalCharacter::SetupCharacter()
{
	// Base setup logic. Not to be used here
}

void AFPSSurvivalCharacter::TakeDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f) return;

	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("%s took damage: %.2f | Remaining Health: %.2f"), *GetName(), DamageAmount, Health);

	if (Health <= 0.0f)
	{
		bIsDead = true;
		Health = 0.0f;

		// Optional: Play death animation, disable movement, etc.
		UE_LOG(LogTemp, Error, TEXT("%s has died."), *GetName());

		// Disable character movement
		AController* MyController = GetController();
		if (MyController)
		{
			MyController->UnPossess();
		}
	}
}
