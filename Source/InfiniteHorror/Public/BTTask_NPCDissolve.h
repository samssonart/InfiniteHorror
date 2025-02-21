// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_NPCDissolve.generated.h"

/**
 * @brief Makes an NPC disappear from the game if they just attacked, with a fade-to-black transition
 */
UCLASS()
class INFINITEHORROR_API UBTTask_NPCDissolve : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_NPCDissolve();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
