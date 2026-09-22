// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatchStreamingManager.generated.h"

class ULevelStreamingDynamic;
class ANavMeshBoundsVolume;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitialPatchesLoaded);

/**
 * @brief Which pool a streamed patch is drawn from, based on the cell's angle relative to the exit direction.
 */
UENUM(BlueprintType)
enum class EPatchCategory : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Good = 1 UMETA(DisplayName = "Good"),
	Neutral = 2 UMETA(DisplayName = "Neutral"),
	Bad = 3 UMETA(DisplayName = "Bad")
};

/**
 * @brief Streams pre-authored terrain patch levels around the player on a fixed XY grid to
 * simulate infinite terrain. Keeps a ring of patches loaded around the player's cell, evicts
 * the farthest patches when the budget is exceeded and picks patches deterministically per cell.
 *
 * On the first update a random ExitDirection is chosen (at least MinExitAngleFromForward degrees
 * away from the player's forward). Each cell is categorised by the dot product between ExitDirection
 * and the direction from the origin cell to that cell: cells toward the exit are Good, cells slightly
 * off it are Neutral and everything else is Bad. The origin cell itself is Neutral.
 */
UCLASS()
class INFINITEHORROR_API APatchStreamingManager : public AActor
{
	GENERATED_BODY()

public:
	APatchStreamingManager();

	/**
	 * @brief Converts a world location into a grid cell coordinate.
	 * @param WorldLocation Location in world space.
	 * @return The cell containing the location.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	FIntPoint WorldToCell(const FVector& WorldLocation) const;

	/**
	 * @brief Converts a grid cell into the world location where its patch level is placed.
	 * @param Cell Grid cell coordinate.
	 * @return World location of the patch level origin.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	FVector CellToWorld(const FIntPoint& Cell) const;

	/**
	 * @brief Classifies a cell by its direction from the origin cell relative to ExitDirection.
	 * @param Cell Grid cell coordinate.
	 * @return Good when the dot product is >= GoodMinDot, Neutral when >= NeutralMinDot, Bad otherwise.
	 * Returns Neutral for the origin cell or before the origin has been established.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	EPatchCategory GetCellCategory(const FIntPoint& Cell) const;

	/**
	 * @brief Deterministically selects which patch from the cell's category pool is used for a cell.
	 * @param Cell Grid cell coordinate.
	 * @return Index into the pool returned by GetPool(GetCellCategory(Cell)), or INDEX_NONE when it is empty.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	int32 SelectPatchIndex(const FIntPoint& Cell) const;

	/**
	 * @brief Normalised XY direction the Good patches lead toward. Zero until the first update.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	FVector2D GetExitDirection() const { return ExitDirection; }

	/**
	 * @brief Cell the player started in and that all category directions are measured from.
	 * @param OutCell Receives the origin cell when established.
	 * @return False before the first update has established the origin.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	bool GetOriginCell(FIntPoint& OutCell) const;

	/**
	 * @brief Forces an immediate re-evaluation of which patches should be loaded.
	 */
	UFUNCTION(BlueprintCallable, Category = "Patch Streaming")
	void UpdatePatches();

	/**
	 * @brief Number of patches currently loaded or loading.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	int32 GetLoadedPatchCount() const { return LoadedPatches.Num(); }

	/**
	 * @brief Sets the location to load the initial ring around before a player pawn exists and
	 * triggers loading immediately if the manager has already begun play.
	 * @param Location World location the player will spawn at.
	 */
	UFUNCTION(BlueprintCallable, Category = "Patch Streaming")
	void RequestInitialLoad(const FVector& Location);

	/**
	 * @brief Whether every patch in the ring around the tracked cell has finished loading and is visible.
	 */
	UFUNCTION(BlueprintPure, Category = "Patch Streaming")
	bool AreInitialPatchesLoaded() const { return bInitialPatchesLoaded; }

	/**
	 * @brief Fired once when the initial ring of patches is fully loaded and visible.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Patch Streaming")
	FOnInitialPatchesLoaded OnInitialPatchesLoaded;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Patch levels used for cells along the exit direction. Each level should contain a Landscape
	 * whose footprint is PatchSize x PatchSize, positioned so that PatchPivotOffset maps it to the cell.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Pool")
	TArray<TSoftObjectPtr<UWorld>> GoodPatchLevels;

	/**
	 * @brief Patch levels used for cells skewed away from the exit direction and for the origin cell.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Pool")
	TArray<TSoftObjectPtr<UWorld>> NeutralPatchLevels;

	/**
	 * @brief Patch levels used for every other cell.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Pool")
	TArray<TSoftObjectPtr<UWorld>> BadPatchLevels;

	/**
	 * @brief Seed mixed into the per-cell hash used for patch selection.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Pool")
	int32 Seed = 0;

	/**
	 * @brief Minimum dot product between ExitDirection and the origin-to-cell direction for a Good cell.
	 * 0.5 corresponds to a 60 degree half-angle cone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Exit", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float GoodMinDot = 0.5f;

	/**
	 * @brief Minimum dot product for a Neutral cell. Cells below this are Bad. Must be <= GoodMinDot.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Exit", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float NeutralMinDot = 0.25f;

	/**
	 * @brief Minimum angle in degrees between the player's initial forward and the randomly chosen ExitDirection.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Exit", meta = (ClampMin = "0.0", ClampMax = "179.0"))
	float MinExitAngleFromForward = 90.0f;

	/**
	 * @brief When true, ExitDirectionOverride is used instead of a random direction. Useful for testing.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Exit")
	bool bOverrideExitDirection = false;

	/**
	 * @brief Fixed XY exit direction used when bOverrideExitDirection is set. Normalised at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Exit", meta = (EditCondition = "bOverrideExitDirection"))
	FVector2D ExitDirectionOverride = FVector2D(1.0, 0.0);

	/**
	 * @brief Size of a single patch along X and Y in world units.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Grid", meta = (ClampMin = "1.0"))
	float PatchSize = 25300.0f;

	/**
	 * @brief Offset added to the cell's min corner when placing a patch level. Use PatchSize / 2
	 * on X and Y if the landscapes were authored centered on the level origin.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Grid")
	FVector PatchPivotOffset = FVector::ZeroVector;

	/**
	 * @brief Chebyshev radius of cells around the player that must be loaded (1 = 3x3 ring).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Grid", meta = (ClampMin = "0"))
	int32 LoadRadius = 1;

	/**
	 * @brief Maximum number of patches kept in the world. Clamped at runtime to at least the ring size.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Budget", meta = (ClampMin = "1"))
	int32 MaxPatches = 12;

	/**
	 * @brief Seconds between player position polls.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Budget", meta = (ClampMin = "0.01"))
	float PollInterval = 0.25f;

	/**
	 * @brief Seconds to wait for the initial ring before broadcasting OnInitialPatchesLoaded anyway.
	 * Prevents waiting players from never spawning when a patch fails to load or become visible.
	 * Set to 0 to wait indefinitely.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Budget", meta = (ClampMin = "0.0"))
	float InitialLoadTimeout = 30.0f;

	/**
	 * @brief NavMesh bounds volume that is moved and resized to cover the loaded ring whenever the player
	 * changes cell. When unset, the first ANavMeshBoundsVolume found in the level is used.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patch Streaming|Navigation")
	TObjectPtr<ANavMeshBoundsVolume> NavBoundsVolume;

	/**
	 * @brief Vertical extent of the nav bounds volume in world units, centred on PatchPivotOffset.Z.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Navigation", meta = (ClampMin = "1.0"))
	float NavBoundsHeight = 20000.0f;

	/**
	 * @brief Draws cell bounds and states on screen.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patch Streaming|Debug")
	bool bDrawDebug = false;

private:
	bool GetTrackedLocation(FVector& OutLocation) const;
	FVector GetTrackedForward() const;
	void EstablishOrigin(const FIntPoint& Cell);
	const TArray<TSoftObjectPtr<UWorld>>& GetPool(EPatchCategory Category) const;
	static const TCHAR* CategoryToString(EPatchCategory Category);
	void LoadPatch(const FIntPoint& Cell);
	void UnloadPatch(const FIntPoint& Cell);
	bool EvictFarthest(const FIntPoint& PlayerCell);
	static int32 CellDistance(const FIntPoint& A, const FIntPoint& B);
	void DrawDebug(const FIntPoint& PlayerCell) const;
	void UpdateNavBounds(const FIntPoint& PlayerCell);
	void CheckInitialPatchesLoaded();
	void HandleInitialLoadTimeout();

	UFUNCTION()
	void HandleLevelShown();

	UPROPERTY(Transient)
	TMap<FIntPoint, TObjectPtr<ULevelStreamingDynamic>> LoadedPatches;

	/** Cells that permanently failed to load (empty pool or load failure); skipped on subsequent polls. */
	TSet<FIntPoint> FailedCells;

	FTimerHandle PollTimerHandle;
	FTimerHandle InitialLoadTimeoutHandle;
	FIntPoint LastPlayerCell = FIntPoint(TNumericLimits<int32>::Max(), TNumericLimits<int32>::Max());
	TOptional<FVector> InitialLoadLocation;
	TOptional<FIntPoint> OriginCell;
	FVector2D ExitDirection = FVector2D::ZeroVector;
	bool bInitialPatchesLoaded = false;
};
