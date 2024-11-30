// Copyright (c) 2024 Samssonart. All rights reserved.


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

		const FString VectorPrintf = FString(TEXT("{0}, {1}, {2}"));

		const FString FormattedVector = FString::Format(*VectorPrintf, { Origin.X, Origin.Y, Origin.Z });

		UE_LOG(LogTemp, Log, TEXT("Origin acquired at %s"), *FormattedVector);

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
				const FString FormattedVector2 = FString::Format(*VectorPrintf, { Location.Location.X, Location.Location.Y, Location.Location.Z });
				UE_LOG(LogTemp, Log, TEXT("Random point acquired at %s"), *FormattedVector2);
				UBlackboardComponent* BBComp =  OwnerComp.GetBlackboardComponent();
				BBComp->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}