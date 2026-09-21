// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCFactory.generated.h"

class APatchStreamingManager;
class ANPCSpirit;
class APlayerCharacter;

UCLASS()
class INFINITEHORROR_API ANPCFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCFactory();

	/*
	 *@brief Leaves the factory ready to spawn another NPC.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetSpawnState();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/*
	* @brief Spawns the first NPC once the initial terrain patches have finished loading.
	*/
	UFUNCTION()
	void OnInitialPatchesLoaded();

private:

	/*
	* @brief Whether the initial terrain patches have finished loading.
	*/
	bool bTerrainReady = false;

	/*
	* @brief The NPC classes that can be spawned.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ANPCSpirit>> NPCRefs;

	/*
	* @brief Vertical offset above the terrain height used when spawning an NPC.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	float NPCHeightOffset = 90.0f;
	
	/*
	* @brief The radius in which NPCs can spawn around the player.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	float NPCSpawnRadius = 500.0f;

	/*
	* @brief The max wait time after a NPC is destroyed to spawn another, in seconds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	float NPCReSpawnDelay = 5.0f;

	/*
	* @brief Half the vertical length of the line trace used to find the terrain height when spawning.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float SpawnTraceHalfHeight = 4000.0f;

	/*
	* @brief Countdown until the next NPC spawn; negative means spawn as soon as possible.
	*/
	float NPCRespawnTimer = -1.0f;

	/*
	* @brief Reference to the player character.
	*/
	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> PlayerActor;

	/*
	* @brief Reference to the NPC currently in play.
	*/
	UPROPERTY(Transient)
	TObjectPtr<ANPCSpirit> NPCInPlay;

	/*
	* @brief Whether an NPC is currently in play.
	*/
	bool bIsNPCInPlay = false;

	/*
	* @brief Spawns an NPC in a random location within the NPCSpawnRadius.
	* @see NPCSpawnRadius
	*/
	void SpawnNPC(int32 NPCIndex);

	/*
	* @brief Spawns an NPC from its class reference.
	* @return The spawned NPC.
	*/
	ANPCSpirit* SpawnNPCFromSubclassRef(UWorld* World, const int32 NPCIndex, const FVector& Location, const FRotator& Rotation);

};

