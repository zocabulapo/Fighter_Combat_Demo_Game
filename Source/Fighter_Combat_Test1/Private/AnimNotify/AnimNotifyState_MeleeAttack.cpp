#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h"
#include "AnimNotify/AnimNotifyState_MeleeAttack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/Character_Base.h"
#include "GameFramework/DamageType.h"
#include "Effect/EffectCombat.h" 

void UAnimNotifyState_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    AlreadyHitActors.Empty();
}

void UAnimNotifyState_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;

    FVector Start = MeshComp->GetSocketLocation(SocketStart);
    FVector End = MeshComp->GetSocketLocation(SocketEnd);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MeshComp->GetOwner());
    Params.bTraceComplex = true;
    Params.bReturnPhysicalMaterial = true;

    bool bHit = MeshComp->GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_GameTraceChannel1,
        FCollisionShape::MakeSphere(TraceRadius),
        Params
    );

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (!HitActor || AlreadyHitActors.Contains(HitActor)) return;

        AlreadyHitActors.Add(HitActor);

        if (ACharacter_Base* HitChar = Cast<ACharacter_Base>(HitActor))
        {
            FPointDamageEvent DamageEvent;
            DamageEvent.Damage = Damage;
            DamageEvent.HitInfo = HitResult;
            DamageEvent.ShotDirection = (End - Start).GetSafeNormal();
            DamageEvent.DamageTypeClass = UDamageType::StaticClass();

            HitChar->TakeDamage(
                Damage,
                DamageEvent,
                MeshComp->GetOwner()->GetInstigatorController(),
                MeshComp->GetOwner()
            );
            if (SkillEffectType != ESkillEffectType::None)
            {
                if (AEffectCombat* EffectCombat = MeshComp->GetWorld()->SpawnActor<AEffectCombat>(AEffectCombat::StaticClass()))
                {
                    FVector HitDirection = (End - Start).GetSafeNormal();
                    float FinalStrength = EffectStrength * FMath::RandRange(0.9f, 1.1f);

                    EffectCombat->ApplyEffect(HitChar, SkillEffectType, HitDirection, FinalStrength);

                    EffectCombat->Destroy();
                }
            }
            if (BuffType != EBuffType::None)
            {
                if (AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner()))
                {
                    if (Player->isFury) 
                    {
                        if (ABase_Buff* BuffActor = MeshComp->GetWorld()->SpawnActor<ABase_Buff>(ABase_Buff::StaticClass()))
                        {
                            BuffActor->InitializeBuff(HitActor, BuffType);
                        }
                    }
                }
            }
        }
        if (AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner()))
        {
            Player->LastHitEnemy = Cast<ACharacter>(HitActor);
        }
        else
        {
            UGameplayStatics::ApplyDamage(
                HitActor,
                Damage,
                MeshComp->GetOwner()->GetInstigatorController(),
                MeshComp->GetOwner(),
                nullptr
            );
        }
    }
}

void UAnimNotifyState_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AlreadyHitActors.Empty();
}
