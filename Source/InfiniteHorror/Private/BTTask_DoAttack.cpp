// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "BTTask_DoAttack.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

UBTTask_DoAttack::UBTTask_DoAttack()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	NodeName = TEXT("Execute Attack");
}

EBTNodeResult::Type UBTTask_DoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BBComp = OwnerComp.GetBlackboardComponent();

	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}
	
	if (!BBComp->GetValueAsBool(GetSelectedBlackboardKey()))
	{
		return EBTNodeResult::Failed;
	}
	
	npc = Cast<ANPC_Spirit>(OwnerComp.GetAIOwner()->GetPawn());
	if (npc)
	{
		npc->Attack();
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_DoAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (npc && BBComp && npc->bHasNPCAttacked)
	{
		BBComp->SetValueAsBool(AttackCompletedKey, true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

