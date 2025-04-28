#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectCombat.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Effect/Base_Buff.h"
#include "AnimNotifyState_MeleeAttack.generated.h"


UCLASS()
class FIGHTER_COMBAT_TEST1_API UAnimNotifyState_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;



protected:
	// Các biến có thể chỉnh trong Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	FName SocketStart = "arm_l";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	FName SocketEnd = "hand_l";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	float TraceRadius = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage= 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
	ESkillEffectType SkillEffectType = ESkillEffectType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect", meta = (EditCondition = "SkillEffectType != ESkillEffectType::None"))
	float EffectStrength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Effect")
	EBuffType BuffType = EBuffType::None;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Effect", meta = (EditCondition = "BuffType != EBuffType::None"))
	float BuffDuration = 5.0f;

	TSet<AActor*> AlreadyHitActors;

};
