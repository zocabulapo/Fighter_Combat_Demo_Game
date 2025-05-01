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
	if (TargetActor == nullptr)
	{
		return;
	}
	switch (BuffType)
	{
	case EBuffType::Burn:
		if (BurnEffect && TargetActor->IsA(ACharacter::StaticClass()))
		{
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

	ElapsedTime += DeltaTime;

	if (ElapsedTime >= Duration)
	{
		RemoveBuff();
	}

	if (ElapsedTime >= 1.0f)
	{
		ApplyDamageOverTime();
		ElapsedTime = 0.0f; 
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
