// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTaskNode_HeavyStun_Task.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_COMBAT_TEST1_API UMyBTTaskNode_HeavyStun_Task : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMyBTTaskNode_HeavyStun_Task();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
