﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ActionSectionControl.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_COMBAT_TEST1_API UAnimNotifyState_ActionSectionControl : public UAnimNotifyState
{
    GENERATED_BODY()

public:

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};