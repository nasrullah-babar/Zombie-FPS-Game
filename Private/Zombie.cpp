#include "ZombieCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

AZombie::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttackRange = 150.f;
	DamageAmount = 20.f;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Play idle or walk montage if needed
	if (WalkMontage) {
		PlayMontage(WalkMontage);
	}
}

void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPlayer)
	{
		float Distance = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

		if (Distance <= AttackRange)
		{
			TryAttackPlayer();
		}
		else
		{
			// Move to player
			FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(Direction, 1.0f);
		}
	}
}

void AZombie::TryAttackPlayer()
{
	if (!GetWorldTimerManager().IsTimerActive(AttackTimer))
	{
		AttackPlayer();

		GetWorldTimerManager().SetTimer(AttackTimer, this, &AZombieCharacter::AttackPlayer, 2.0f, false);
	}
}

void AZombie::AttackPlayer()
{
	if (AttackMontage)
	{
		PlayMontage(AttackMontage);
	}

	if (TargetPlayer)
	{
		UGameplayStatics::ApplyDamage(TargetPlayer, DamageAmount, GetController(), this, nullptr);
	}
}

void AZombie::Die()
{
	PlayMontage(DeathMontage);
	// Disable collision, movement, etc.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void AZombieCharacter::PlayMontage(UAnimMontage* Montage)
{
	if (Montage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}
