// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Buff.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	None     UMETA(DisplayName = "None"), // Không có buff
	Burn     UMETA(DisplayName = "Burn"),
	Poison   UMETA(DisplayName = "Poison"),
	Freeze   UMETA(DisplayName = "Freeze"),
	Heal     UMETA(DisplayName = "Heal"),
};

UCLASS()
class FIGHTER_COMBAT_TEST1_API ABase_Buff : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Buff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Đối tượng nhận Buff
	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	AActor* TargetActor;

	// Loại Buff (Burn, Poison, v.v.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	EBuffType BuffType;

	// Thời gian hiệu lực của Buff
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	float Duration = 5.0f;

	// Thêm thuộc tính Damage mỗi giây
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	float TickDamage = 5.0f;

	// Thời gian đã trôi qua
	float ElapsedTime = 0.0f;

	// Gọi khi Buff được áp dụng
	void InitializeBuff(AActor* Target, EBuffType Type);

	// Hàm áp dụng Buff
	void ApplyBuff();

	// Hàm áp dụng Damage theo thời gian
	void ApplyDamageOverTime();

	// Hàm gỡ bỏ Buff
	void RemoveBuff();

private:
	// Các hiệu ứng particle (Ví dụ: lửa cho Burn)
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* BurnEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PoisonEffect;

	// Hiệu ứng Particle hiện tại
	UParticleSystemComponent* ActiveEffect;
};
