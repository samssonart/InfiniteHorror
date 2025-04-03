// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralFoliageComponent.h"
#include "ProceduralFoliageVolume.h"
#include "FoliageManager.generated.h"

UCLASS()
class INFINITEHORROR_API AFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageManager();

	/*
	 *@brief The pool of different terrain patches to spawn
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Terrain")
	TArray<TSubclassOf<AActor>> TerrainPatchPool;

	/*
	 *@brief The relative squared distance of the player to spawn a new terrain patch
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Terrain")
	float SpawnDistanceThreshold = 5000.0f;

	/*
	* @brief The actual PFG component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PFG")
	TObjectPtr<UProceduralFoliageComponent> ProceduralFoliageGenerator;

	/*
	* @brief The actor that contains the procedural foliage volume
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PFG")
	TObjectPtr<AActor> ProceduralFoliageVolume;

	/*
	 * @brief Reference to the Material Parameter Collection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* MaterialParameterCollection;

	/*
	 * @brief Max squared distance the player can move before the fog re-centers
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog", meta = (AllowPrivateAccess = "true"))
	float MaxFogRecenterDistanceSqr;

	/*
	* @brief The foliage info
	*/
	FFoliageInfo FoliageInfo;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnPatchAtLocation(FVector Location);
	
	float _fogSqrDisplacement = 0.0f;
	FVector _previousPlayerFogPosition;
	TObjectPtr<ACharacter> _mainCharacter;
	TObjectPtr<UMaterialParameterCollectionInstance> _MPC_Instance;
	TObjectPtr<AActor> _currentTerrainPatch;
};
