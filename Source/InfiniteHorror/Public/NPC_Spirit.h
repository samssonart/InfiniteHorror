// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "NPC_Spirit.generated.h"


UCLASS()
class INFINITEHORROR_API ANPC_Spirit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit ANPC_Spirit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviourTree = nullptr;

	UBehaviorTree* GetBehaviorTree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* PawnSensor = nullptr;

	UPawnSensingComponent* GetPawnSensor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
