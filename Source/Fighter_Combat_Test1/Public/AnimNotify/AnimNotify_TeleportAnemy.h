// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_TeleportAnemy.generated.h"

UCLASS()
class FIGHTER_COMBAT_TEST1_API UAnimNotify_TeleportAnemy : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
    float TeleportDistance = 100.0f; // Khoảng cách cách enemy sau khi teleport

};
