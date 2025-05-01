#include "Animation/Player_AnimationBlueprint.h"
#include "Fighter_Combat_Test1/Fighter_Combat_Test1Character.h" // include file header nhân vật đúng tên
void UPlayer_AnimationBlueprint::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<ACharacter>(TryGetPawnOwner());
    if (Character)
    {
        MovementComponent = Character->GetCharacterMovement();
    }
}

void UPlayer_AnimationBlueprint::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character || !MovementComponent)
    {
        return;
    }

    // Update Velocity
    Velocity = MovementComponent->Velocity;
    GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();

    FVector Acceleration = MovementComponent->GetCurrentAcceleration();
    bool bHasInput = !Acceleration.IsNearlyZero();
    bShouldMove = GroundSpeed > 3.0f && bHasInput;

    FRotator Rotation = Character->GetActorRotation();
    FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
    Direction = CalculateDirection(Velocity, Rotation);

    bool bRealFalling = MovementComponent->IsFalling();

    AFighter_Combat_Test1Character* MyPlayer = Cast<AFighter_Combat_Test1Character>(Character);
    if (MyPlayer)
    {
        isAttacking = MyPlayer->isAttacking;
    }

    if (isAttacking)
    {
        bIsFalling = false;
    }
    else
    {
        bIsFalling = bRealFalling;
    }
}
