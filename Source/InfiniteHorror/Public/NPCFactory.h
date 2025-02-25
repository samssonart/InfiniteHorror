// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Spirit.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "NPCFactory.generated.h"

UCLASS()
class INFINITEHORROR_API ANPCFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCFactory();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

private:

	/*
	* @brief The paths of the NPCs that can be spawned.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ANPC_Spirit>> NPCRefs;

	/*
	* @brief The pool of NPCs that can be spawned.
	*/
	TArray<TObjectPtr<ANPC_Spirit>> NPCPool;

	/*
	* @brief The radius of the NPC's sight
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	float NPCHeightOffset = 90.0f;
	
	/*
	* @brief The radius in which NPCs can spawn around the player.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs", meta = (AllowPrivateAccess = "true"))
	float NPCSpawnRadius = 500.0f;

	/*
	* @brief Reference to the player character.
	*/
	TObjectPtr<APlayerCharacter> PlayerActor;

	/*
	* @brief Reference to the player character.
	*/
	TObjectPtr<ANPC_Spirit> NPCInPlay;

	/*
	* @brief Whether an NPC is currently in play.
	*/
	bool bIsNPCInPlay = false;

	/*
	* @brief Spawns an NPC from the NPC pool in a random location within the NPCSpawnRadius.
	* @see NPCSPawnRadius
	*/
	void SpawnNPC(int NPCIndex);

	/*
	* @biefs Spawns an NPC blueprint from a its game path.
	* @return The spawned NPC.
	*/
	ANPC_Spirit* SpawnNPCFromSubclassRef(UWorld* World, const int NPCIndex, const FVector& Location, const FRotator& Rotation);

};
