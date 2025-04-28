// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_ApproachEnemy.h"
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Character_Base.h"

void UAnimNotifyState_ApproachEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
		if (!MeshComp) return;

		ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
		if (!Character) return;

		Character->bUseControllerRotationYaw = false;
		Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void UAnimNotifyState_ApproachEnemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!MeshComp) return;

	AFighter_Combat_Test1Character* Character = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner());
	if (!Character || !Character->GetWorld()) return;

	ACharacter* ClosestEnemy = nullptr;
	float ClosestDistSq = FLT_MAX;
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(Character->GetWorld(), ACharacter_Base::StaticClass(), FoundEnemies);

	for (AActor* Actor : FoundEnemies)
	{
		if (Actor == Character) continue;

		float DistSq = FVector::DistSquared(Actor->GetActorLocation(), Character->GetActorLocation());
		if (DistSq < ClosestDistSq && DistSq <= FMath::Square(ApproachRange))
		{
			ClosestDistSq = DistSq;
			ClosestEnemy = Cast<ACharacter>(Actor);
		}
	}

	if (ClosestEnemy)
	{
		FVector ToEnemy = (ClosestEnemy->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
		Character->AddMovementInput(ToEnemy, MoveSpeed * FrameDeltaTime);

		// Blend xoay siêu mượt
		FRotator CurrentRot = Character->GetActorRotation();
		FRotator TargetRot = FRotationMatrix::MakeFromX(ToEnemy).Rotator();
		TargetRot.Pitch = 0.f;
		TargetRot.Roll = 0.f;

		// Tăng tốc độ blend mượt hơn
		const float RotationInterpSpeed = 12.0f; // Tùy chỉnh max mượt - 12 tới 15f là đẹp
		FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetRot, FrameDeltaTime, RotationInterpSpeed);
		Character->SetActorRotation(SmoothRot);

		// Đồng bộ mượt luôn Controller
		AController* Controller = Character->GetController();
		if (Controller)
		{
			FRotator ControlSmoothRot = FMath::RInterpTo(Controller->GetControlRotation(), TargetRot, FrameDeltaTime, RotationInterpSpeed);
			ControlSmoothRot.Pitch = 0.f;
			ControlSmoothRot.Roll = 0.f;
			Controller->SetControlRotation(ControlSmoothRot);
		}
	}
}

void UAnimNotifyState_ApproachEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	Character->bUseControllerRotationYaw = true;
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}
