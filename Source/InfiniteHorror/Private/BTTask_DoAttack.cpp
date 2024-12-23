// Copyright (c) 2024 Samssonart. All rights reserved.


#include "BTTask_DoAttack.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UBTTask_DoAttack::UBTTask_DoAttack()
{
	NodeName = TEXT("Execute Attack");
}

EBTNodeResult::Type UBTTask_DoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool withinRange = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (!withinRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	ANPC_Spirit* const npc = Cast<ANPC_Spirit>(OwnerComp.GetAIOwner()->GetPawn());

	if (npc && HasMontageFinishedPlaying(npc))
	{
		npc->Attack();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_DoAttack::HasMontageFinishedPlaying(ANPC_Spirit* const NPC)
{
	return NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NPC->GetAnimationMontage());
}

