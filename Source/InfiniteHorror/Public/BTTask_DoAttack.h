// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPC_spirit.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_DoAttack.generated.h"

/**
 * @brief Makes an NPC attack the player when the player is within attack range
 */
UCLASS()
class INFINITEHORROR_API UBTTask_DoAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_DoAttack();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	bool bAttackMontagePlayedAlready = false;
	bool HasMontageFinishedPlaying(ANPC_Spirit* const NPC);
	bool HasMontagePlayed(ANPC_Spirit* const NPC);

	TObjectPtr<UAnimInstance> AnimInstance;
	TObjectPtr<ANPC_Spirit> npc;
	TObjectPtr<UBlackboardComponent> BBComp;

	/*
	* @brief The name of the BB key that sets the state to attack completed
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	FName AttackCompletedKey = "NPCAttacked";

	
};
