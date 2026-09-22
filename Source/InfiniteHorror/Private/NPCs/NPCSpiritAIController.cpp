// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/NPCSpiritAIController.h"
#include "NPCs/NPCSpirit.h"
#include "Player/PlayerCharacter.h"
#include "InfiniteHorrorLog.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
ANPCSpiritAIController::ANPCSpiritAIController()
{
    SetupStimuli();
}

void ANPCSpiritAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPCSpirit* const NPC = Cast<ANPCSpirit>(InPawn))
	{
		if (UBehaviorTree* const Tree = NPC->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComp;
			UseBlackboard(Tree->BlackboardAsset, BlackboardComp);
			Blackboard = BlackboardComp;
			RunBehaviorTree(Tree);
		}

		SetSightParameters(this, NPC->SightRadius, NPC->LoseSightOffset, NPC->PeripheralVisionAngle);
	}
}

void ANPCSpiritAIController::SetupStimuli()
{
    SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    if (SightConfiguration)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		// Initialize at general purpose values
		SightConfiguration->SightRadius = 1000.0f;
		SightConfiguration->LoseSightRadius = 1150.0f;
		SightConfiguration->PeripheralVisionAngleDegrees = 80.0f;
		SightConfiguration->SetMaxAge(5.f);
		SightConfiguration->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
		SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;
		
        GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCSpiritAIController::OnPlayerDetected); 
		GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
    }

}

void ANPCSpiritAIController::OnPlayerDetected(AActor* DetectedActor, FAIStimulus const Stimulus)
{
    // Only the player character should trip the PlayerDetected flag; other NPCs are characters too.
    if (!Cast<APlayerCharacter>(DetectedActor))
    {
        return;
    }

    if (UBlackboardComponent* const BBComp = GetBlackboardComponent())
    {
        BBComp->SetValueAsBool("PlayerDetected", Stimulus.WasSuccessfullySensed());
    }
}

UAISenseConfig* ANPCSpiritAIController::GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass)
{
	UAISenseConfig* Result = nullptr;

	const FAISenseID SenseID = UAISense::GetSenseID(SenseClass);
	if (!SenseID.IsValid())
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("GetPerceptionSenseConfig: Wrong Sense ID"));
	}
	else if (Controller == nullptr)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("GetPerceptionSenseConfig: Controller is NULL!"));
	}
	else
	{
		UAIPerceptionComponent* Perception = Controller->GetAIPerceptionComponent();
		if (Perception == nullptr)
		{
			UE_LOG(LogInfiniteHorror, Error, TEXT("GetPerceptionSenseConfig: Perception is NULL!"));
		}
		else
		{
			Result = Perception->GetSenseConfig(SenseID);
		}
	}

	return Result;
}

bool ANPCSpiritAIController::SetSightParameters(AAIController* Controller, float SightRange, float LoseSightOffset, float PeripheralAngle)
{
	UAISenseConfig_Sight* ConfigSight = Cast<UAISenseConfig_Sight>(
		GetPerceptionSenseConfig(Controller, UAISense_Sight::StaticClass()));
	UAIPerceptionComponent* Perception = Controller ? Controller->GetAIPerceptionComponent() : nullptr;
	if (!ConfigSight || !Perception)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("SetSightParams: missing sight config or perception component!"));
		return false;
	}

	ConfigSight->SightRadius = SightRange;
	ConfigSight->LoseSightRadius = ConfigSight->SightRadius + LoseSightOffset;
	ConfigSight->PeripheralVisionAngleDegrees = PeripheralAngle;
	Perception->RequestStimuliListenerUpdate();

	return true;
}


