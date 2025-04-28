// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ApproachEnemy.generated.h"

UCLASS(meta = (DisplayName = "Approach Enemy Over Time"))
class FIGHTER_COMBAT_TEST1_API UAnimNotifyState_ApproachEnemy : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Approach")
	float ApproachRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Approach")
	float MoveSpeed = 600.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
