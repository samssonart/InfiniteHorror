// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#include "BTTask_NPCDissolve.h"
#include "AIController.h"
#include "NPC_spirit.h"
#include "Runtime/Engine/Classes/Engine/World.h"

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

	bool npcJustAttacked = BBComp->GetValueAsBool(GetSelectedBlackboardKey());
	if (npcJustAttacked)
	{
		TObjectPtr<ANPC_Spirit> const npc = Cast<ANPC_Spirit>(OwnerComp.GetAIOwner()->GetPawn());
		if (npc)
		{
			// FString InfoMessage = FString::Printf(TEXT("My work here is done... *dissolves*"));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InfoMessage);
			npc->Destroy();
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::InProgress;
}