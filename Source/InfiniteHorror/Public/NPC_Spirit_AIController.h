// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NPC_Spirit_AIController.generated.h"


UCLASS()
class INFINITEHORROR_API ANPC_Spirit_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit ANPC_Spirit_AIController();

	UPawnSensingComponent* PawnSensor;

	float DetectionCooldown = 2.5f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool playerDetected = false;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

	//////////////////////////////////////////////////////////////////////////
	// UPawnSensingComponent Delegates

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

private:

	bool _playerDetected;
};
