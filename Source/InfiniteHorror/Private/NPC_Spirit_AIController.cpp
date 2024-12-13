// Copyright (c) 2024 Samssonart. All rights reserved.


#include "NPC_Spirit_AIController.h"
#include "NPC_Spirit.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
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

		this->PawnSensor = npc->GetPawnSensor();

	}
}

// Called when the game starts or when spawned
void ANPC_Spirit_AIController::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensor)
	{
		FString message = TEXT("Pawn Sensor is valid, creating callbacks");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		PawnSensor->OnSeePawn.AddDynamic(this, &ANPC_Spirit_AIController::OnSeePawn);
		PawnSensor->OnHearNoise.AddDynamic(this, &ANPC_Spirit_AIController::OnHearNoise);
	}
}

// Called every frame
void ANPC_Spirit_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC_Spirit_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ANPC_Spirit_AIController::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("I HEAR YOU!!! ") + OtherActor->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TODO: game-specific logic    
}

void ANPC_Spirit_AIController::OnSeePawn(APawn* OtherPawn)
{
	_playerDetected = true;
	GetBlackboardComponent()->SetValueAsBool("PlayerDetected", true);
}


