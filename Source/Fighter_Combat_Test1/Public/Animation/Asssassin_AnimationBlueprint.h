#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Asssassin_AnimationBlueprint.generated.h"

UCLASS()
class FIGHTER_COMBAT_TEST1_API UAsssassin_AnimationBlueprint : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Hàm override để update biến mỗi frame
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// Tốc độ di chuyển
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	// Kiểm tra nhân vật đang bay trên không hay không
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;
};
