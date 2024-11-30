// Copyright (c) 2024 Samssonart. All rights reserved.


#include "NPC_Spirit.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ANPC_Spirit::ANPC_Spirit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_Spirit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPC_Spirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBehaviorTree* ANPC_Spirit::GetBehaviorTree()
{
	return BehaviourTree;
}

