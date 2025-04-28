#include "Animation/Asssassin_AnimationBlueprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UAsssassin_AnimationBlueprint::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;

	// Lấy tốc độ từ velocity
	Speed = OwnerPawn->GetVelocity().Size();

	// Kiểm tra nhân vật có đang bay không
	ACharacter* Character = Cast<ACharacter>(OwnerPawn);
	if (Character)
	{
		bIsInAir = Character->GetMovementComponent()->IsFalling();
	}
}
