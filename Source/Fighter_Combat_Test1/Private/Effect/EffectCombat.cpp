// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Effect/EffectCombat.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

AEffectCombat::AEffectCombat()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEffectCombat::BeginPlay()
{
    Super::BeginPlay();
}

void AEffectCombat::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEffectCombat::ApplyEffect(ACharacter* TargetCharacter, ESkillEffectType EffectType, FVector Direction, float Strength)
{
    if (!TargetCharacter) return;

    switch (EffectType)
    {
    case ESkillEffectType::LaunchUp:
        LaunchUp(TargetCharacter, Strength);
        break;
    case ESkillEffectType::PushBack:
        PushBack(TargetCharacter, Direction, Strength);
        break;
    case ESkillEffectType::SmashDown:
        SmashDown(TargetCharacter, Strength);
        break;
    case ESkillEffectType::None:
    default:
        break;
    }
}

void AEffectCombat::LaunchUp(ACharacter* Target, float Strength)
{
    if (!Target) return;

    FVector Forward = Target->GetActorForwardVector();
    Forward.Z = 0.f;
    Forward.Normalize();

    float ForwardRatio = 0.4f; 
    FVector LaunchVelocity = Forward * (Strength * ForwardRatio) + FVector(0.f, 0.f, Strength);

    Target->LaunchCharacter(LaunchVelocity, true, true);
}

void AEffectCombat::PushBack(ACharacter* Target, FVector Direction, float Strength)
{
    if (!Target) return;

    if (Direction.IsNearlyZero())
    {
        Direction = Target->GetActorForwardVector() * -1.f;
    }
    Direction.Normalize();

    FVector LaunchVelocity = Direction * Strength;
    Target->LaunchCharacter(LaunchVelocity, true, true);
}

void AEffectCombat::SmashDown(ACharacter* Target, float Strength)
{
    if (!Target) return;
    UCharacterMovementComponent* MoveComp = Target->GetCharacterMovement();
    if (MoveComp)
    {
        MoveComp->GravityScale = 1.75f;
    }
    FVector LaunchVelocity = FVector(0.f, 0.f, -Strength);
    Target->LaunchCharacter(LaunchVelocity, true, true);
}
