// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotify_Control_Gravity.h"
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_Control_Gravity::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!OwnerCharacter) return;
    if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
    {
        Movement->GravityScale = GravityScale;

        if (FMath::IsNearlyZero(GravityScale))
        {
            Movement->Velocity.Z = 0.0f; 
        }
        else
        {
            if (Movement->MovementMode == MOVE_None)
            {
                Movement->SetMovementMode(MOVE_Falling); 
            }
        }
    }
    if (AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(OwnerCharacter))
    {
        ACharacter* LastEnemy = Player->LastHitEnemy;
        if (LastEnemy)
        {
            if (UCharacterMovementComponent* EnemyMovement = LastEnemy->GetCharacterMovement())
            {
                EnemyMovement->GravityScale = GravityScale;

                if (FMath::IsNearlyZero(GravityScale))
                {
                    EnemyMovement->Velocity.Z = 0.0f; 
                }
                else
                {
                    if (EnemyMovement->MovementMode == MOVE_None)
                    {
                        EnemyMovement->SetMovementMode(MOVE_Falling); 
                    }
                }
            }
        }
        Player->LastHitEnemy = nullptr;
    }
}
