#include "Character/Character_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

ACharacter_Base::ACharacter_Base()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHP = MaxHP;

    // Cho phép MeleeTrace đụng trúng Mesh
    GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

    // Không cho MeleeTrace đụng Capsule
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}
void ACharacter_Base::BeginPlay()
{
    Super::BeginPlay();
}

void ACharacter_Base::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ACharacter_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    const FPointDamageEvent* PointEvent = DamageEvent.IsOfType(FPointDamageEvent::ClassID)
        ? static_cast<const FPointDamageEvent*>(&DamageEvent)
        : nullptr;

    UPhysicalMaterial* PhysMat = nullptr;

    if (PointEvent)
    {
        const FHitResult& Hit = PointEvent->HitInfo;
        PhysMat = Hit.PhysMaterial.IsValid() ? Hit.PhysMaterial.Get() : nullptr;

        FName MatName = PhysMat ? PhysMat->GetFName() : NAME_None;

        // 🎯 Nếu trúng đầu → nhân đôi damage
        if (MatName == FName("HeadShoot"))
        {
            ActualDamage *= 2.f;
        }

        DrawDebugString(
            GetWorld(),
            Hit.ImpactPoint + FVector(0, 0, 30),
            FString::Printf(TEXT("PhysMat: %s | Damage: %.2f"), *MatName.ToString(), ActualDamage),
            nullptr,
            FColor::Red,
            2.f,
            true
        );
    }

    // Cập nhật HP
    CurrentHP -= ActualDamage;
    CurrentHP = FMath::Clamp(CurrentHP, 0.0f, MaxHP);

    UE_LOG(LogTemp, Warning, TEXT("[TakeDamage] Final Damage: %.2f | Current HP: %.2f"), ActualDamage, CurrentHP);

    if (CurrentHP <= 0.0f)
    {
        Die();
        return ActualDamage;
    }

    // Tính StunLevel
    int32 StunLevel = 0;
    if (ActualDamage >= HeavyStunThreshold)      StunLevel = 2;
    else if (ActualDamage >= LightStunThreshold) StunLevel = 1;

    // Gửi sang Blackboard
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        UBlackboardComponent* BB = AIController->GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsBool("isStunned", StunLevel > 0);
            BB->SetValueAsInt("StunLevel", StunLevel);
            BB->SetValueAsFloat("LastDamage", ActualDamage);
            BB->SetValueAsObject("PhysMatHit", PhysMat);
            BB->SetValueAsObject("DamageCauser", DamageCauser);
        }
    }

    return ActualDamage;
}
void ACharacter_Base::Die()
{
    if (isDead)
    {
        // Nếu nhân vật đã chết, không làm gì thêm
        return;
    }
    isDead = true;
    if (DeathMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(DeathMontage);
        }
    }
}
