// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CheckCombo.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_COMBAT_TEST1_API UAnimNotifyState_CheckCombo : public UAnimNotifyState
{
	GENERATED_BODY()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	FName NextLightSection = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combo")
	FName NextHeavySection = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bRequireEnemyHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float LightStaminaCost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float HeavyStaminaCost = 25.f;


};
