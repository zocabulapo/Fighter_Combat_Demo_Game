// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fighter_Combat_Test1Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFighter_Combat_Test1Character

AFighter_Combat_Test1Character::AFighter_Combat_Test1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFighter_Combat_Test1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
void AFighter_Combat_Test1Character::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		StaminaRegenTimer,
		this,
		&AFighter_Combat_Test1Character::RegenerateStamina,
		1.0f,
		true
	);

}

void AFighter_Combat_Test1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighter_Combat_Test1Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFighter_Combat_Test1Character::Look);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AFighter_Combat_Test1Character::PerformHeavyAttack);
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AFighter_Combat_Test1Character::PerformLightAttack);

		EnhancedInputComponent->BindAction(FuryModeAction, ETriggerEvent::Triggered, this, &AFighter_Combat_Test1Character::FuryMode);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFighter_Combat_Test1Character::Move(const FInputActionValue& Value)
{
	if (!canMove || Controller == nullptr) return; 
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}


void AFighter_Combat_Test1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AFighter_Combat_Test1Character::PerformHeavyAttack()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (GetCharacterMovement()->IsFalling() && !isAttacking)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("CannotHeavy"));
		return;
	}
	if (IsPlayingMontage(LightAttackMontage))
	{
		ComboInput = EComboInputType::HEAVY;
		return;
	}
	if (isAttacking)
	{
		ComboInput = EComboInputType::HEAVY;
		return;
	}
	const float Speed = GetVelocity().Size();
	isAttacking = true;
	if (AirAttackMontage && Speed > 500.f && !IsPlayingMontage(AirAttackMontage))
	{
		AnimInstance->Montage_Play(AirAttackMontage);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("Started Air Heavy Attack!"));
	}
	else if (HeavyAttackMontage && !IsPlayingMontage(HeavyAttackMontage))
	{
		AnimInstance->Montage_Play(HeavyAttackMontage);
	}
	else
	{
		ComboInput = EComboInputType::HEAVY;
	}
}


void AFighter_Combat_Test1Character::PerformLightAttack()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (GetCharacterMovement()->IsFalling() && !isAttacking)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Cannot Light"));
		return;
	}
	if (IsPlayingMontage(HeavyAttackMontage))
	{
		ComboInput = EComboInputType::LIGHT;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued LIGHT during heavy combo."));
		return;
	}
	if (isAttacking)
	{
		ComboInput = EComboInputType::LIGHT;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued LIGHT Combo Input"));
		return;
	}
	if (LightAttackMontage && !IsPlayingMontage(LightAttackMontage))
	{
		AnimInstance->Montage_Play(LightAttackMontage);
		isAttacking = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Performed Light Attack!"));
	}
	else
	{
		ComboInput = EComboInputType::LIGHT;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("LightAttackMontage is null or already playing."));
	}
}



void AFighter_Combat_Test1Character::ResetCombo()
{
	ComboInput = EComboInputType::NONE;
}


bool AFighter_Combat_Test1Character::IsPlayingMontage(UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	return AnimInstance && Montage && AnimInstance->Montage_IsPlaying(Montage);
}
void AFighter_Combat_Test1Character::RegenerateStamina()
{
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate, 0.f, MaxStamina);
	}
}


void AFighter_Combat_Test1Character::Dead()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (!isDead) 
	{
		isDead = true;
		// Phát FuryMontage
		if (DeadMontage) 
		{
			AnimInstance->Montage_Play(DeadMontage);
		}

	}
}

void AFighter_Combat_Test1Character::FuryMode()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (!isFury)
	{
		isFury = true;

		// Phát FuryMontage
		if (FuryMontage) 
		{
			AnimInstance->Montage_Play(FuryMontage);
		}
		GetWorld()->GetTimerManager().SetTimer(FuryTimerHandle, [this]()
			{
				isFury = false;  
			}, 50.0f, false);
	}
}
