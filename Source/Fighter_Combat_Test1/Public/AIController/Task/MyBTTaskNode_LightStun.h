// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTaskNode_LightStun.generated.h"

UCLASS()
class FIGHTER_COMBAT_TEST1_API UMyBTTaskNode_LightStun : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTaskNode_LightStun();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
