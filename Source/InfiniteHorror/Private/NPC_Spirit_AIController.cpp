// Copyright (c) 2024 Samssonart. All rights reserved.


#include "NPC_Spirit_AIController.h"
#include "NPC_Spirit.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ANPC_Spirit_AIController::ANPC_Spirit_AIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SetupStimuli();
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

void ANPC_Spirit_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ANPC_Spirit_AIController::SetupStimuli()
{
    SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    if (SightConfiguration)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
        SightConfiguration->SightRadius = 5000.f;
        SightConfiguration->LoseSightRadius = SightConfiguration->SightRadius + 50.f;
        SightConfiguration->PeripheralVisionAngleDegrees = 80.f;
        SightConfiguration->SetMaxAge(5.f);
        SightConfiguration->AutoSuccessRangeFromLastSeenLocation = 520.f;
        SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
        SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_Spirit_AIController::OnPlayerDetected);
        GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
    }

}

void ANPC_Spirit_AIController::OnPlayerDetected(AActor* DetectedActor, FAIStimulus const Stimulus)
{
    if (auto* const player = Cast<ACharacter>(DetectedActor))
    {
        GetBlackboardComponent()->SetValueAsBool("PlayerDetected", Stimulus.WasSuccessfullySensed());
    }
}



