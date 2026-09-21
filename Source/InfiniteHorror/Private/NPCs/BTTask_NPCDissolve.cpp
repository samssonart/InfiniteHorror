// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#include "NPCs/BTTask_NPCDissolve.h"
#include "AIController.h"
#include "NPCs/NPCSpirit.h"

UBTTask_NPCDissolve::UBTTask_NPCDissolve()
{
	NodeName = TEXT("Dissolve NPC");
}

EBTNodeResult::Type UBTTask_NPCDissolve::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<UBlackboardComponent> BBComp = OwnerComp.GetBlackboardComponent();

	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	const bool bNPCJustAttacked = BBComp->GetValueAsBool(GetSelectedBlackboardKey());
	if (bNPCJustAttacked)
	{
		AAIController* const AICont = OwnerComp.GetAIOwner();
		ANPCSpirit* const NPC = AICont ? Cast<ANPCSpirit>(AICont->GetPawn()) : nullptr;
		if (NPC)
		{
			NPC->StartDissolve();
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// No tick notification is enabled, so returning InProgress would hang the tree forever.
	return EBTNodeResult::Failed;
}
