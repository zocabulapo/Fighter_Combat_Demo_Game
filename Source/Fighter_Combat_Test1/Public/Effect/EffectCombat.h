#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectCombat.generated.h"

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
    None        UMETA(DisplayName = "None"),
    LaunchUp    UMETA(DisplayName = "Launch Up"),
    PushBack    UMETA(DisplayName = "Push Back"),
    SmashDown   UMETA(DisplayName = "Smash Down")
};

UCLASS()
class FIGHTER_COMBAT_TEST1_API AEffectCombat : public AActor
{
    GENERATED_BODY()

public:
    AEffectCombat();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    /** Gọi để apply hiệu ứng */
    UFUNCTION(BlueprintCallable, Category = "Skill Effect")
    void ApplyEffect(ACharacter* TargetCharacter, ESkillEffectType EffectType, FVector Direction = FVector::ZeroVector, float Strength = 1000.f);

private:
    void LaunchUp(ACharacter* Target, float Strength);
    void PushBack(ACharacter* Target, FVector Direction, float Strength);
    void SmashDown(ACharacter* Target, float Strength);
};
