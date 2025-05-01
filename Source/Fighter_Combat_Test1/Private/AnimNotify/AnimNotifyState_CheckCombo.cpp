#include "AnimNotify/AnimNotifyState_CheckCombo.h"
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h" 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_CheckCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner());
    if (!Player) return;
    if (bRequireEnemyHit && Player->ComboInput != EComboInputType::LIGHT && !IsValid(Player->LastHitEnemy))
    {
        return;
    }

    UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    if (AnimInstance && Animation)
    {
        FName TargetSection = NAME_None;
        float Cost = 0.f;

        switch (Player->ComboInput)
        {
        case EComboInputType::LIGHT:
            TargetSection = NextLightSection;
            Cost = LightStaminaCost;
            break;
        case EComboInputType::HEAVY:
            TargetSection = NextHeavySection;
            Cost = HeavyStaminaCost;
            break;
        default:
            break;
        }
        if (Cost > 0.f && Player->CurrentStamina < Cost)
        {
            return;
        }

        if (TargetSection != NAME_None)
        {
            AnimInstance->Montage_JumpToSection(TargetSection, Cast<UAnimMontage>(Animation));
            Player->CurrentStamina = FMath::Max(0.f, Player->CurrentStamina - Cost);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No combo input."));
        }

        Player->ResetCombo();
    }
}
