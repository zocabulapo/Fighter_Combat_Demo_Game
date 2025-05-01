#include "AIController/Task/MyBTTaskNode_HeavyStun_Task.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Character_Base.h"
#include "Animation/AnimInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UMyBTTaskNode_HeavyStun_Task::UMyBTTaskNode_HeavyStun_Task()
{
	NodeName = "Heavy Stun Montage";
}

EBTNodeResult::Type UMyBTTaskNode_HeavyStun_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacter_Base* Character = Cast<ACharacter_Base>(AIController->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	int32 StunLevel = BB->GetValueAsInt("StunLevel");
	if (StunLevel < 2) return EBTNodeResult::Failed; 
	UObject* PhysMatObj = BB->GetValueAsObject("PhysMatHit");
	UPhysicalMaterial* PhysMat = Cast<UPhysicalMaterial>(PhysMatObj);
	if (!PhysMat) return EBTNodeResult::Failed;

	FName MatName = PhysMat->GetFName();

	if (!Character->StunMontages.Contains(MatName))
	{
		return EBTNodeResult::Failed;
	}

	const FStunMontageData& StunData = Character->StunMontages[MatName];
	UAnimMontage* Montage = StunData.HeavyStun;

	if (!Montage)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;
	AnimInstance->Montage_Play(Montage);

	return EBTNodeResult::Succeeded;
}
