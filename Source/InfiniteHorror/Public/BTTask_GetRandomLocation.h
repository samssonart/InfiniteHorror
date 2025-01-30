// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BTTask_GetRandomLocation.generated.h"

/**
 * Finds a random location within a specified radius
 */
UCLASS()
class INFINITEHORROR_API UBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTTask_GetRandomLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/*
	* @brief Reference to the scene's navigation system
	*/
	TObjectPtr<UNavigationSystemV1> NavSystem;

	/*
	* @brief Whether the navigation system has been set
	* @see NavSystem
	*/
	bool NavigationInvokerSet = false;

	/*
	* @brief The radius within which to search for a random location
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	float SearchRadius = 15.0f;
	
};
