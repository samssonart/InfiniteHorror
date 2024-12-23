// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_GetPlayerPosition.generated.h"

/**
 * Finds the first player actor and gets its position
 */
UCLASS()
class INFINITEHORROR_API UBTTask_GetPlayerPosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_GetPlayerPosition(FObjectInitializer const& ObjecctInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float PlayerOffset = 10.0f;
};
