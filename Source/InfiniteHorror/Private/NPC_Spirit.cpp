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
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = 0.5f;
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(75.f);
	PawnSensor->SightRadius = 3000.0f;
	PawnSensor->HearingThreshold = 800.0f;
	PawnSensor->LOSHearingThreshold = 1600.0f;
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

UPawnSensingComponent* ANPC_Spirit::GetPawnSensor()
{
	return PawnSensor;
}

UBehaviorTree* ANPC_Spirit::GetBehaviorTree()
{
	return BehaviourTree;
}

