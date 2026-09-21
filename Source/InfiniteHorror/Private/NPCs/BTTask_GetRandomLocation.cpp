// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/BTTask_GetRandomLocation.h"
#include "NPCs/NPCSpiritAIController.h"
#include "NavigationSystem.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get a Random Location within a Nav Mesh";
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCSpiritAIController* const AICon = Cast<ANPCSpiritAIController>(OwnerComp.GetAIOwner());
	APawn* const Pawn = AICon ? AICon->GetPawn() : nullptr;
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();
	if (!Pawn || !NavSystem || !BBComp)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation Location;
	if (!NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, Location))
	{
		return EBTNodeResult::Failed;
	}

	BBComp->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
	return EBTNodeResult::Succeeded;
}
