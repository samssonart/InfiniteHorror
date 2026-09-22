// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NPCSpiritAIController.generated.h"


UCLASS()
class INFINITEHORROR_API ANPCSpiritAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this controller's properties
	ANPCSpiritAIController();

	static UAISenseConfig* GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintCallable)
	static bool SetSightParameters(AAIController* Controller, float SightRange, float LoseSightOffset, float PeripheralAngle);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Sight> SightConfiguration;

	void SetupStimuli();

	UFUNCTION()
	void OnPlayerDetected(AActor* DetectedActor, FAIStimulus const Stimulus);
};

