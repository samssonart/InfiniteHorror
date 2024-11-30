// Copyright (c) 2024 Samssonart. All rights reserved.


#include "NPC_Spirit_AIController.h"
#include "NPC_Spirit.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ANPC_Spirit_AIController::ANPC_Spirit_AIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ANPC_Spirit_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC_Spirit* const npc = Cast<ANPC_Spirit>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* blackBoard;
			UseBlackboard(tree->BlackboardAsset, blackBoard);
			Blackboard = blackBoard;
			RunBehaviorTree(tree);
		}
	}
}

// Called when the game starts or when spawned
void ANPC_Spirit_AIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPC_Spirit_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


