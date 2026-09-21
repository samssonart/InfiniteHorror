// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/BTService_ChangeCharacterSpeed.h"
#include "AIController.h"
#include "NPCs/NPCSpirit.h"
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
		if (auto* const NPC = Cast<ANPCSpirit>(AICont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}

