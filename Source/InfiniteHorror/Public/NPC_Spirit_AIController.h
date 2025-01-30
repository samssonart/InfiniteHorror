// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "NPC_Spirit_AIController.generated.h"


UCLASS()
class INFINITEHORROR_API ANPC_Spirit_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit ANPC_Spirit_AIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static UAISenseConfig* GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintCallable)
	static bool SetSightParameters(AAIController* Controller, float SightRange, float LoseSightOffset, float PeripheralAngle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool playerDetected = false;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

private:

	class UAISenseConfig_Sight* SightConfiguration;
	class UAISenseConfig_Hearing* HearingConfiguration;

	void SetupStimuli();

	UFUNCTION()
	void OnPlayerDetected(AActor* DetectedActor, FAIStimulus const Stimulus);
};
