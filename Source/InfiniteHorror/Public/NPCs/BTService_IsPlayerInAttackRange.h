// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEHORROR_API UBTService_IsPlayerInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsPlayerInAttackRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 20.0f;
};
