// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Control_Gravity.generated.h"

UCLASS()
class FIGHTER_COMBAT_TEST1_API UAnimNotify_Control_Gravity : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityScale = 1.0f;
};
