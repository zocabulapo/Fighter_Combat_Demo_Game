#include "GameFramework/CharacterMovementComponent.h"
#include "AnimNotify/AnimNotifyState_ActionSectionControl.h"
#include "GameFramework/Character.h" 
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h"

void UAnimNotifyState_ActionSectionControl::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner());
    if (Player)
    {
        Player->canMove = false;
        Player->isAttacking = true;
    }
}

void UAnimNotifyState_ActionSectionControl::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AFighter_Combat_Test1Character* Player = Cast<AFighter_Combat_Test1Character>(MeshComp->GetOwner());
    if (Player)
    {
        Player->canMove = true;
        Player->isAttacking = false;
    }
}
