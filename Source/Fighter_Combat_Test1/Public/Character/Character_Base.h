#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h" 
#include "Character_Base.generated.h" 

USTRUCT(BlueprintType)
struct FStunMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LightStun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HeavyStun;
};
UCLASS()
class FIGHTER_COMBAT_TEST1_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor
	ACharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHP = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float CurrentHP;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	TMap<FName, FStunMontageData> StunMontages;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float LightStunThreshold = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float HeavyStunThreshold = 50.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isDead = false;
	virtual void Die();
};
