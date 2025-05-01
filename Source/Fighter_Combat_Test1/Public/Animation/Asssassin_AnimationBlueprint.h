#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Asssassin_AnimationBlueprint.generated.h"

UCLASS()
class FIGHTER_COMBAT_TEST1_API UAsssassin_AnimationBlueprint : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;
};
