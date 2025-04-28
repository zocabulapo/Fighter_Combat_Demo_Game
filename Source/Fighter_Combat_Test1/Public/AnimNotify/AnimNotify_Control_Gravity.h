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
    // Hàm override Notify để thi triển hiệu ứng
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    // 🛸 Tham số Gravity Scale để truyền vào (default = 1.0 bình thường)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityScale = 1.0f;
};