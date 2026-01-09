// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPSSurvivalWeaponComponent.generated.h"

class AFPSSurvivalCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSURVIVAL_API UFPSSurvivalWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	bool operator==(const UFPSSurvivalWeaponComponent& Other) const;

	// Getter and Setter for Encapsulation
	FName GetWeaponType() const;
	void SetWeaponType(FName NewType);

	bool AttachWeapon(AFPSSurvivalCharacter* TargetCharacter);


	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSSurvivalProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Sets default values for this component's properties */
	UFPSSurvivalWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AFPSSurvivalCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AFPSSurvivalProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UInputMappingContext* FireMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector MuzzleOffset;

	UPROPERTY()
	AFPSSurvivalCharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponType;
protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AFPSSurvivalCharacter* Character;
};
