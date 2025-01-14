// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


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

		SetSightParameters(this, npc->SightRadius, npc->LoseSightOffset, npc->PeripheralVisionAngle);
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

UAISenseConfig* ANPC_Spirit_AIController::GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass)
{
	UAISenseConfig* result = nullptr;

	FAISenseID Id = UAISense::GetSenseID(SenseClass);
	if (!Id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Wrong Sense ID"));
	}
	else if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Controller is NULL!"));
	}
	else
	{
		UAIPerceptionComponent* Perception = Controller->GetAIPerceptionComponent();
		if (Perception == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Perception is NULL!"));
		}
		else
		{
			result = Perception->GetSenseConfig(Id);
		}
	}

	return result;
}

bool ANPC_Spirit_AIController::SetSightParameters(AAIController* Controller, float SightRange, float LoseSightOffset, float PeripheralAngle)
{
	UAISenseConfig* Config = GetPerceptionSenseConfig(Controller, UAISense_Sight::StaticClass());
	if (Config == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetSightParams: Config is NULL!"));
		return false;
	}
	else
	{
		UAISenseConfig_Sight* ConfigSight = Cast<UAISenseConfig_Sight>(Config);

		ConfigSight->SightRadius = SightRange;
		ConfigSight->LoseSightRadius = ConfigSight->SightRadius + LoseSightOffset;
		ConfigSight->PeripheralVisionAngleDegrees = PeripheralAngle;

		UAIPerceptionComponent* Perception = Controller->GetAIPerceptionComponent();
		Perception->RequestStimuliListenerUpdate();
	}

	return true;
}

