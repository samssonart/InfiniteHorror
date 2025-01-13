// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPC_spirit.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_DoAttack.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEHORROR_API UBTTask_DoAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_DoAttack();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
	bool HasMontageFinishedPlaying(ANPC_Spirit* const NPC);
	
};
