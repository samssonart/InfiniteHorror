// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeCharacterSpeed.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEHORROR_API UBTService_ChangeCharacterSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTService_ChangeCharacterSpeed();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float speed = 600.0f;
	
};
