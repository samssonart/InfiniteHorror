// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/BTTask_DoAttack.h"
#include "NPCs/NPCSpirit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DoAttack::UBTTask_DoAttack()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	NodeName = TEXT("Execute Attack");
}

EBTNodeResult::Type UBTTask_DoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp || !BBComp->GetValueAsBool(GetSelectedBlackboardKey()))
	{
		return EBTNodeResult::Failed;
	}

	AAIController* const AICont = OwnerComp.GetAIOwner();
	ANPCSpirit* const NPC = AICont ? Cast<ANPCSpirit>(AICont->GetPawn()) : nullptr;
	if (!NPC)
	{
		return EBTNodeResult::Failed;
	}

	NPC->Attack();
	return EBTNodeResult::InProgress;
}

void UBTTask_DoAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Re-fetch per-execution state from OwnerComp: node instances are shared across pawns.
	UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();
	AAIController* const AICont = OwnerComp.GetAIOwner();
	ANPCSpirit* const NPC = AICont ? Cast<ANPCSpirit>(AICont->GetPawn()) : nullptr;

	if (NPC && BBComp && NPC->bHasNPCAttacked)
	{
		BBComp->SetValueAsBool(AttackCompletedKey, true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


