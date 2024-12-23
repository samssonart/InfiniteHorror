// Copyright (c) 2024 Samssonart. All rights reserved.


#include "BTService_IsPlayerInAttackRange.h"
#include "NPC_Spirit.h"
#include "NPC_Spirit_AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player Within Attack Range");
}


void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPC_Spirit_AIController* const AICont = Cast<ANPC_Spirit_AIController>(OwnerComp.GetAIOwner());
	ANPC_Spirit* const NPC = Cast<ANPC_Spirit>(AICont->GetPawn());
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NPC->GetDistanceTo(player) <= AttackRange);
}
