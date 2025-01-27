// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgetController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "NPC_Spirit.generated.h"

/*
 * Common class for all NPC (spirits) in the game
 */
UCLASS()
class INFINITEHORROR_API ANPC_Spirit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit ANPC_Spirit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviourTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float LoseSightOffset = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngle = 75.0f;

	UBehaviorTree* GetBehaviorTree();

	UAnimMontage* GetAnimationMontage() const;

	void Attack();
	void AttackStart();
	void AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	float AttackValue = 30.0f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	FVector HandCollisionOffset;

	ACharacter* PlayerActor;
	
	UUIWidgetController* WidgetController = nullptr;

	bool IsRotating = false;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult);
	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex);
};
