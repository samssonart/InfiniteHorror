// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "BTTask_GetRandomLocation.h"
#include "NPC_Spirit_AIController.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get a Random Location within a Nav Mesh";

	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ANPC_Spirit_AIController* const npc = Cast<ANPC_Spirit_AIController>(OwnerComp.GetAIOwner()))
	{
		FVector const Origin = npc->GetPawn()->GetActorLocation();

		if (NavSystem)
		{
			if (!NavigationInvokerSet)
			{
				NavSystem->RegisterNavigationInvoker(npc->GetPawn()->GetOwner(), SearchRadius, SearchRadius);
				NavigationInvokerSet = true;
			}

			FNavLocation Location;
			if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
			{
				UBlackboardComponent* BBComp =  OwnerComp.GetBlackboardComponent();
				BBComp->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}