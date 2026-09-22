// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "NPCSpirit.generated.h"

class UBehaviorTree;
class UAnimMontage;
class UGameplayAbility;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;
class UDifficultyAbilitySystemComponent;
class UNavigationInvokerComponent;
class UBoxComponent;
class UHUDWidget;
class ANPCFactory;

/*
 * Common class for all NPC (spirits) in the game
 */
UCLASS()
class INFINITEHORROR_API ANPCSpirit : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCSpirit();

	/*
	* @brief Whether the NPC has detected the player and is within attack distance
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC State")
	bool bIsNPCInAttackMode = false;

	/*
	* @brief Whether the NPC has already attacked the player
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC State")
	bool bHasNPCAttacked = false;

	/*
	* @brief Whether the NPC is currently playing its dissolve transition
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC State")
	bool bIsNPCDissolving = false;

	/*
	* @brief The behavior tree that the NPC's AI controller will use
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

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
	TObjectPtr<UMaterialParameterCollection> MaterialParameterCollection;

	/*
	 * @brief The actual instance of the Material Parameter Collection
	 */
	UPROPERTY(Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;

	/*
	* @brief Returns the behavior tree
	* @return The behavior tree
	* @see BehaviorTree
	*/
	UBehaviorTree* GetBehaviorTree();

	/*
	* @brief Returns the animation montage
	* @return The animation montage
	*/
	UAnimMontage* GetAnimationMontage() const;

	/*
	* @brief Performs the NPC attack
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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	/*
	 * @brief Sets the default abilities of the NPC
	 */
	void SetDefaultAbilities();
	
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage>  AnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> HandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	FVector HandCollisionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

	/*
	* @brief Keeps navmesh tiles generated around this NPC so it can navigate anywhere in the streamed world
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNavigationInvokerComponent> NavigationInvoker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> PlayerActor;
	
	UPROPERTY(Transient)
	TObjectPtr<UHUDWidget> WidgetController = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<ANPCFactory> NPCFactoryRef = nullptr;

	bool bIsRotating = false;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, const int32 OtherBodyIndex, const bool bFromSweep, FHitResult const& SweepResult);
	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, const int32 OtherBodyIndex);
};

