// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/BTService_IsPlayerInAttackRange.h"
#include "NPCs/NPCSpirit.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player Within Attack Range");
}


void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* const AICont = OwnerComp.GetAIOwner();
	ANPCSpirit* const NPC = AICont ? Cast<ANPCSpirit>(AICont->GetPawn()) : nullptr;
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();

	if (!NPC || !Player || !BBComp)
	{
		return;
	}

	BBComp->SetValueAsBool(GetSelectedBlackboardKey(), NPC->GetDistanceTo(Player) <= AttackRange);
}

