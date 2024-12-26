// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
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

	UAnimMontage* GetAnimationMontage() const;

	void Attack();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.0f;

	ACharacter* PlayerActor;

	bool IsRotating = false;
};
