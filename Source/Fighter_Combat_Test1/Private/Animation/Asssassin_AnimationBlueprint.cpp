#include "Animation/Asssassin_AnimationBlueprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UAsssassin_AnimationBlueprint::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;

	Speed = OwnerPawn->GetVelocity().Size();

	ACharacter* Character = Cast<ACharacter>(OwnerPawn);
	if (Character)
	{
		bIsInAir = Character->GetMovementComponent()->IsFalling();
	}
}
