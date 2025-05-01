// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/Basic_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ABasic_AIController::ABasic_AIController()
{
}

void ABasic_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp;
		UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp);
		RunBehaviorTree(BehaviorTree);
	}
}

