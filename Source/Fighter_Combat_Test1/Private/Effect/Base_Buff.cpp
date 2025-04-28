#include "Effect/Base_Buff.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"

// Constructor
ABase_Buff::ABase_Buff()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

void ABase_Buff::BeginPlay()
{
	Super::BeginPlay();
}

void ABase_Buff::InitializeBuff(AActor* Target, EBuffType Type)
{
	TargetActor = Target;
	BuffType = Type;
	ApplyBuff();
}

void ABase_Buff::ApplyBuff()
{
	// Kiểm tra nếu TargetActor là ACharacter
	if (TargetActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is null!"));
		return;
	}

	// Áp dụng Buff theo loại
	switch (BuffType)
	{
	case EBuffType::Burn:
		if (BurnEffect && TargetActor->IsA(ACharacter::StaticClass()))
		{
			// Gọi hiệu ứng lửa cho Burn
			ActiveEffect = UGameplayStatics::SpawnEmitterAttached(
				BurnEffect,
				Cast<ACharacter>(TargetActor)->GetMesh(),
				FName("EffectSocket"),
				FVector(0, 0, 0),
				FRotator(0, 0, 0),
				EAttachLocation::Type::SnapToTarget,
				true
			);
		}
		break;

	case EBuffType::Poison:
		if (PoisonEffect && TargetActor->IsA(ACharacter::StaticClass()))
		{
			// Gọi hiệu ứng nọc độc cho Poison
			ActiveEffect = UGameplayStatics::SpawnEmitterAttached(
				PoisonEffect,
				Cast<ACharacter>(TargetActor)->GetMesh(),
				FName("EffectSocket"),
				FVector(0, 0, 0),
				FRotator(0, 0, 0),
				EAttachLocation::Type::SnapToTarget,
				true
			);
		}
		break;

	default:
		break;
	}
}

void ABase_Buff::ApplyDamageOverTime()
{
	// Nếu có đối tượng target thì tiến hành giảm HP
	if (TargetActor)
	{
		if (ACharacter* Character = Cast<ACharacter>(TargetActor))
		{
			// Áp dụng sát thương cho đối tượng (kẻ địch hoặc player)
			UGameplayStatics::ApplyDamage(TargetActor, TickDamage, nullptr, nullptr, nullptr);
		}
	}
}

void ABase_Buff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Cập nhật thời gian
	ElapsedTime += DeltaTime;

	// Kiểm tra nếu thời gian đã đủ để gỡ bỏ buff
	if (ElapsedTime >= Duration)
	{
		RemoveBuff();
	}

	// Áp dụng sát thương theo thời gian mỗi 1 giây
	if (ElapsedTime >= 1.0f)
	{
		ApplyDamageOverTime();
		ElapsedTime = 0.0f; // Reset lại thời gian sau mỗi lần áp dụng sát thương
	}
}


void ABase_Buff::RemoveBuff()
{
	// Gỡ bỏ hiệu ứng
	if (ActiveEffect)
	{
		ActiveEffect->DestroyComponent();
	}
}
