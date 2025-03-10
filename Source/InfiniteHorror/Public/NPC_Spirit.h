// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgetController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "NPC_Spirit.generated.h"

class ANPCFactory;

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

	/*
	* @brief Whether the NPC has detected the player and is within attack distance
	*/
	bool bIsNPCInAttackMode = false;

	/*
	* @brief Whether the NPC has already attacked the player
	*/
	bool bHasNPCAttacked = false;

	/*
	* @brief Whether the NPC has already attacked the player
	*/
	bool bIsNPCDissolving = false;

	/*
	* @bried The behavior tree that the NPC' will follow's AI controller will use
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree>  BehaviourTree = nullptr;

	/*
	* @brief The radius of the NPC's sight
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Setup", meta = (AllowPrivateAccess = "true"))
	float NPCHeight = 100.0f;
	
	/*
	* @brief The radius of the NPC's sight
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float SightRadius = 2000.0f;

	/*
	* @brief The radius beyond which the NPC will lose sight of the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float LoseSightOffset = 200.0f;

	/*
	* @brief The angle of the NPC's peripheral vision
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngle = 75.0f;

	/*
	* @brief The amount of damage the NPC's attack will cause
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	float AttackValue = 30.0f;

	/*
	* @brief The amount of time the dissolve transition needs to complete, in seconds
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Setup", meta = (AllowPrivateAccess = "true"))
	float DissolveDuration = 6.0f;

	/*
	 * @brief Reference to the Material Parameter Collection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Setup", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* MaterialParameterCollection;

	/*
	 *@brief The actual instance of the Material Parameter Collection
	 */
	TObjectPtr<UMaterialParameterCollectionInstance> MPC_Instance;

	/*
	* @brief Returns the behavior tree
	* @return The behavior tree
	* @see BehaviourTree
	*/
	UBehaviorTree* GetBehaviorTree();

	/*
	* @brief Returns the animation montage
	* @return The animation montage
	*/
	UAnimMontage* GetAnimationMontage() const;

	/*
	* @bried Performs the NPC attack
	*/
	void Attack();

	/*
	* @brief Receives the message that the NPC should attack
	*/
	void AttackStart();

	/*
	* @brief Receives the message that the NPC's attack animation finished
	*/
	void AttackEnd();

	/*
	* @brief Starts the dissolve transition
	*/
	void StartDissolve();

	/*
	* @brief Modifies all materials in the NPC to create the dissolve transition
	*/
	void SetDissolveAmount(float DissolveAmount);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage>  AnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	FVector HandCollisionOffset;

	TObjectPtr<ACharacter> PlayerActor;
	
	TObjectPtr<UUIWidgetController> WidgetController = nullptr;

	TObjectPtr<ANPCFactory> NPCFactoryRef = nullptr;

	bool IsRotating = false;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult);
	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex);
};
