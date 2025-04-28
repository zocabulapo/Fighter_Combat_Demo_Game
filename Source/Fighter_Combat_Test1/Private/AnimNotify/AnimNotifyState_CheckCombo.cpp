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
        UE_LOG(LogTemp, Warning, TEXT("❌ No enemy hit — combo blocked."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Combo Input Detected: %d"), (uint8)Player->ComboInput);

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

        // ❗ Check stamina trước khi combo tiếp
        if (Cost > 0.f && Player->CurrentStamina < Cost)
        {
            UE_LOG(LogTemp, Warning, TEXT("❌ Not enough stamina to continue combo."));
            return;
        }

        if (TargetSection != NAME_None)
        {
            AnimInstance->Montage_JumpToSection(TargetSection, Cast<UAnimMontage>(Animation));
            Player->CurrentStamina = FMath::Max(0.f, Player->CurrentStamina - Cost);

            UE_LOG(LogTemp, Warning, TEXT("✅ Jumped to section: %s | 🔋 Stamina: %.1f"), *TargetSection.ToString(), Player->CurrentStamina);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("⚠️ No target section set for combo input."));
        }

        Player->ResetCombo();
    }
}