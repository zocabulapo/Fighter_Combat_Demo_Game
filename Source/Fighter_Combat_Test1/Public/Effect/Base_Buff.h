// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Buff.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	None     UMETA(DisplayName = "None"), 
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

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	AActor* TargetActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	EBuffType BuffType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	float Duration = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	float TickDamage = 5.0f;

	float ElapsedTime = 0.0f;

	void InitializeBuff(AActor* Target, EBuffType Type);

	void ApplyBuff();

	void ApplyDamageOverTime();

	void RemoveBuff();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* BurnEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PoisonEffect;

	UParticleSystemComponent* ActiveEffect;
};
