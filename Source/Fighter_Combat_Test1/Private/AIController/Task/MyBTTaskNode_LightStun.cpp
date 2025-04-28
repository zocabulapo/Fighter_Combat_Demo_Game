#include "AIController/Task/MyBTTaskNode_LightStun.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Character_Base.h"
#include "Animation/AnimInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UMyBTTaskNode_LightStun::UMyBTTaskNode_LightStun()
{
	NodeName = "Light Stun Montage";
}

EBTNodeResult::Type UMyBTTaskNode_LightStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacter_Base* Character = Cast<ACharacter_Base>(AIController->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	int32 StunLevel = BB->GetValueAsInt("StunLevel");
	if (StunLevel != 1) return EBTNodeResult::Failed; // Đây là task stun nhẹ

	UObject* PhysMatObj = BB->GetValueAsObject("PhysMatHit");
	UPhysicalMaterial* PhysMat = Cast<UPhysicalMaterial>(PhysMatObj);
	if (!PhysMat) return EBTNodeResult::Failed;

	FName MatName = PhysMat->GetFName();

	if (!Character->StunMontages.Contains(MatName)) return EBTNodeResult::Failed;

	const FStunMontageData& StunData = Character->StunMontages[MatName];
	UAnimMontage* Montage = StunData.LightStun;

	if (!Montage) return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	AnimInstance->Montage_Play(Montage);
	UE_LOG(LogTemp, Warning, TEXT("[LightStun] Played: %s for PhysMat: %s"), *Montage->GetName(), *MatName.ToString());

	return EBTNodeResult::Succeeded;
}
