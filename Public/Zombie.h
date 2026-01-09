#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"

UCLASS()
classI AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();

protected:
	virtual void BeginPlay() override;

	// Animation Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* WalkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* IdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathMontage;

	// Attack logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageAmount;

	FTimerHandle AttackTimer;

	APawn* TargetPlayer;

	void TryAttackPlayer();

	void PlayMontage(UAnimMontage* Montage);

public:
	virtual void Tick(float DeltaTime) override;

	// Call this from Blueprint or AI when attacking
	UFUNCTION(BlueprintCallable)
	void AttackPlayer();

	UFUNCTION(BlueprintCallable)
	void Die();
};
