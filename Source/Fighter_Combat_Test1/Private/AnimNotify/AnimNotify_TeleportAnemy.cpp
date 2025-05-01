// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_TeleportAnemy.h"
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_TeleportAnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    {
        if (!MeshComp) return;

        ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
        if (!OwnerCharacter) return;

        AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(OwnerCharacter);
        if (!Player) return;

        ACharacter* Target = Player->LastHitEnemy;
        if (!Target) return;

        FVector TargetLocation = Target->GetActorLocation();

        FVector BackOffset = Target->GetActorForwardVector() * -100.0f;
        FVector TeleportLocation = TargetLocation + BackOffset;

        Player->SetActorLocation(TeleportLocation, false, nullptr, ETeleportType::TeleportPhysics);

        UCharacterMovementComponent* TargetMovement = Target->GetCharacterMovement();
        if (TargetMovement)
        {
            TargetMovement->DisableMovement();
        }

        UCharacterMovementComponent* PlayerMovement = Player->GetCharacterMovement();
        if (PlayerMovement)
        {
            PlayerMovement->GravityScale = 0.0f; 
        }
    }
}
