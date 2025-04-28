// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Basic_AIController.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_COMBAT_TEST1_API ABasic_AIController : public AAIController
{
	GENERATED_BODY()
	public:
	ABasic_AIController();
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	protected:

		virtual void OnPossess(APawn* InPawn) override;


};
