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
	if (!canMove || Controller == nullptr) return; // <- check ở đây

	// input là Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Tính toán hướng di chuyển
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

	// Nếu đang trên không mà không phải đang attacking => Không cho phép đánh
	if (GetCharacterMovement()->IsFalling() && !isAttacking)
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Cannot heavy attack while falling if not already attacking."));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("❌ Can't heavy attack midair unless attacking already!"));
		return;
	}

	// Nếu đang chơi montage light → queue heavy
	if (IsPlayingMontage(LightAttackMontage))
	{
		ComboInput = EComboInputType::HEAVY;
		UE_LOG(LogTemplateCharacter, Log, TEXT("Queued HEAVY during light combo."));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued HEAVY during light combo."));
		return;
	}

	// Nếu đang combo heavy rồi → queue tiếp
	if (isAttacking)
	{
		ComboInput = EComboInputType::HEAVY;
		UE_LOG(LogTemplateCharacter, Log, TEXT("Queued HEAVY Combo Input"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued HEAVY Combo Input"));
		return;
	}

	// Tới đây thì cho Play montage mới
	const float Speed = GetVelocity().Size();
	isAttacking = true;

	// Nếu đang bay nhanh → chơi Air Attack
	if (AirAttackMontage && Speed > 500.f && !IsPlayingMontage(AirAttackMontage))
	{
		AnimInstance->Montage_Play(AirAttackMontage);
		UE_LOG(LogTemplateCharacter, Log, TEXT("Started Air Heavy Attack!"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("Started Air Heavy Attack!"));
	}
	else if (HeavyAttackMontage && !IsPlayingMontage(HeavyAttackMontage))
	{
		AnimInstance->Montage_Play(HeavyAttackMontage);
		UE_LOG(LogTemplateCharacter, Log, TEXT("Performed Ground Heavy Attack!"));
	}
	else
	{
		ComboInput = EComboInputType::HEAVY;
		UE_LOG(LogTemplateCharacter, Warning, TEXT("HeavyAttackMontage is null or already playing."));
	}
}


void AFighter_Combat_Test1Character::PerformLightAttack()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;

	// Nếu đang trên không mà không phải đang attacking => Không cho phép đánh
	if (GetCharacterMovement()->IsFalling() && !isAttacking)
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Cannot attack while falling if not already attacking."));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("❌ Can't attack midair unless attacking already!"));
		return;
	}

	// Nếu đang trong heavy combo → chỉ queue input
	if (IsPlayingMontage(HeavyAttackMontage))
	{
		ComboInput = EComboInputType::LIGHT;
		UE_LOG(LogTemplateCharacter, Log, TEXT("Queued LIGHT during heavy combo."));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued LIGHT during heavy combo."));
		return;
	}

	// Nếu đang combo light rồi → queue
	if (isAttacking)
	{
		ComboInput = EComboInputType::LIGHT;
		UE_LOG(LogTemplateCharacter, Log, TEXT("Queued LIGHT Combo Input"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Queued LIGHT Combo Input"));
		return;
	}

	// Nếu tới đây thì OK để Play Montage
	if (LightAttackMontage && !IsPlayingMontage(LightAttackMontage))
	{
		AnimInstance->Montage_Play(LightAttackMontage);
		isAttacking = true;

		UE_LOG(LogTemplateCharacter, Log, TEXT("Performed Light Attack!"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Performed Light Attack!"));
	}
	else
	{
		ComboInput = EComboInputType::LIGHT;
		UE_LOG(LogTemplateCharacter, Warning, TEXT("LightAttackMontage is null or already playing."));
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
	// 🔍 Debug log mỗi lần gọi để chắc chắn Timer đang chạy
	UE_LOG(LogTemp, Log, TEXT("⏱ RegenerateStamina() called"));

	// Log BEFORE regen
	UE_LOG(LogTemp, Log, TEXT("📊 BEFORE: Current = %.1f / Max = %.1f"), CurrentStamina, MaxStamina);

	// Log lên màn hình
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Yellow,
			FString::Printf(TEXT("🕒 Regen Tick: %.1f / %.1f"), CurrentStamina, MaxStamina)
		);
	}

	// Thực hiện regen
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate, 0.f, MaxStamina);
	}
}


void AFighter_Combat_Test1Character::Dead()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (!isDead) // <-- Check lại cho chắc
	{
		isDead = true;
		// Phát FuryMontage
		if (DeadMontage)  // Kiểm tra nếu montage có tồn tại
		{
			AnimInstance->Montage_Play(DeadMontage);
		}

	}
}

void AFighter_Combat_Test1Character::FuryMode()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return;
	if (!isFury) // Kiểm tra nếu chưa trong chế độ Fury
	{
		isFury = true;

		// Phát FuryMontage
		if (FuryMontage)  // Kiểm tra nếu montage có tồn tại
		{
			AnimInstance->Montage_Play(FuryMontage);
		}

		// Set một bộ hẹn giờ để tắt Fury sau 50 giây
		GetWorld()->GetTimerManager().SetTimer(FuryTimerHandle, [this]()
			{
				isFury = false;  // Dừng chế độ Fury sau 50 giây
			}, 50.0f, false);
	}
}
