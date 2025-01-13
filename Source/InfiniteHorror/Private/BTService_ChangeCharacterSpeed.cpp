// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "BTService_ChangeCharacterSpeed.h"
#include "AIController.h"
#include "NPC_Spirit.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTService_ChangeCharacterSpeed::UBTService_ChangeCharacterSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Character Speed");
}

void UBTService_ChangeCharacterSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto const AICont = OwnerComp.GetAIOwner())
	{
		if (auto* const NPC = Cast<ANPC_Spirit>(AICont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = speed;
		}
	}
}
