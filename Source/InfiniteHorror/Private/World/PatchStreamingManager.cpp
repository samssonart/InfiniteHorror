// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "World/PatchStreamingManager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/PlayerCameraManager.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/BrushComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPatchStreaming, Log, All);

APatchStreamingManager::APatchStreamingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APatchStreamingManager::BeginPlay()
{
	Super::BeginPlay();

	const auto IsNullLevel = [](const TSoftObjectPtr<UWorld>& Level) { return Level.IsNull(); };
	GoodPatchLevels.RemoveAll(IsNullLevel);
	NeutralPatchLevels.RemoveAll(IsNullLevel);
	BadPatchLevels.RemoveAll(IsNullLevel);
	if (GoodPatchLevels.IsEmpty() && NeutralPatchLevels.IsEmpty() && BadPatchLevels.IsEmpty())
	{
		UE_LOG(LogPatchStreaming, Error, TEXT("%s: All patch pools are empty, no terrain will be streamed."), *GetName());
		bInitialPatchesLoaded = true;
		OnInitialPatchesLoaded.Broadcast();
		return;
	}
	if (GoodPatchLevels.IsEmpty() || NeutralPatchLevels.IsEmpty() || BadPatchLevels.IsEmpty())
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: One or more patch pools are empty (Good: %d, Neutral: %d, Bad: %d). Cells in empty categories will not be streamed."),
			*GetName(), GoodPatchLevels.Num(), NeutralPatchLevels.Num(), BadPatchLevels.Num());
	}

	if (NeutralMinDot > GoodMinDot)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: NeutralMinDot (%.2f) is greater than GoodMinDot (%.2f). Clamping."),
			*GetName(), NeutralMinDot, GoodMinDot);
		NeutralMinDot = GoodMinDot;
	}

	const int32 RingSize = FMath::Square(LoadRadius * 2 + 1);
	if (MaxPatches < RingSize)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: MaxPatches (%d) is smaller than the load ring (%d). Clamping."),
			*GetName(), MaxPatches, RingSize);
		MaxPatches = RingSize;
	}

	if (!NavBoundsVolume)
	{
		for (TActorIterator<ANavMeshBoundsVolume> It(GetWorld()); It; ++It)
		{
			NavBoundsVolume = *It;
			break;
		}
	}
	if (!NavBoundsVolume)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: No ANavMeshBoundsVolume found. NPC navigation will not follow the player."), *GetName());
	}
	else if (UBrushComponent* Brush = NavBoundsVolume->GetBrushComponent())
	{
		Brush->SetMobility(EComponentMobility::Movable);
	}

	UpdatePatches();
	GetWorldTimerManager().SetTimer(PollTimerHandle, this, &APatchStreamingManager::UpdatePatches, PollInterval, true);

	if (InitialLoadTimeout > 0.0f)
	{
		GetWorldTimerManager().SetTimer(InitialLoadTimeoutHandle, this, &APatchStreamingManager::HandleInitialLoadTimeout, InitialLoadTimeout, false);
	}
}

void APatchStreamingManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(PollTimerHandle);
	GetWorldTimerManager().ClearTimer(InitialLoadTimeoutHandle);

	TArray<FIntPoint> Cells;
	LoadedPatches.GenerateKeyArray(Cells);
	for (const FIntPoint& Cell : Cells)
	{
		UnloadPatch(Cell);
	}

	Super::EndPlay(EndPlayReason);
}

FIntPoint APatchStreamingManager::WorldToCell(const FVector& WorldLocation) const
{
	return FIntPoint(
		FMath::FloorToInt32(WorldLocation.X / PatchSize),
		FMath::FloorToInt32(WorldLocation.Y / PatchSize));
}

FVector APatchStreamingManager::CellToWorld(const FIntPoint& Cell) const
{
	return FVector(Cell.X * PatchSize, Cell.Y * PatchSize, 0.0) + PatchPivotOffset;
}

void APatchStreamingManager::RequestInitialLoad(const FVector& Location)
{
	InitialLoadLocation = Location;
	if (HasActorBegunPlay())
	{
		UpdatePatches();
	}
}

bool APatchStreamingManager::GetOriginCell(FIntPoint& OutCell) const
{
	if (!OriginCell.IsSet())
	{
		return false;
	}
	OutCell = OriginCell.GetValue();
	return true;
}

EPatchCategory APatchStreamingManager::GetCellCategory(const FIntPoint& Cell) const
{
	if (!OriginCell.IsSet() || Cell == OriginCell.GetValue())
	{
		return EPatchCategory::Neutral;
	}

	const FVector2D ToCell = FVector2D(Cell.X - OriginCell->X, Cell.Y - OriginCell->Y).GetSafeNormal();
	const double Dot = FVector2D::DotProduct(ToCell, ExitDirection);
	if (Dot >= GoodMinDot)
	{
		return EPatchCategory::Good;
	}
	if (Dot >= NeutralMinDot)
	{
		return EPatchCategory::Neutral;
	}
	return EPatchCategory::Bad;
}

int32 APatchStreamingManager::SelectPatchIndex(const FIntPoint& Cell) const
{
	const TArray<TSoftObjectPtr<UWorld>>& Pool = GetPool(GetCellCategory(Cell));
	if (Pool.IsEmpty())
	{
		return INDEX_NONE;
	}

	const uint32 Hash = FCrc::MemCrc32(&Cell, sizeof(FIntPoint), static_cast<uint32>(Seed));
	return static_cast<int32>(Hash % static_cast<uint32>(Pool.Num()));
}

const TArray<TSoftObjectPtr<UWorld>>& APatchStreamingManager::GetPool(EPatchCategory Category) const
{
	switch (Category)
	{
	case EPatchCategory::Good: return GoodPatchLevels;
	case EPatchCategory::Bad: return BadPatchLevels;
	default: return NeutralPatchLevels;
	}
}

const TCHAR* APatchStreamingManager::CategoryToString(EPatchCategory Category)
{
	switch (Category)
	{
	case EPatchCategory::Good: return TEXT("Good");
	case EPatchCategory::Bad: return TEXT("Bad");
	default: return TEXT("Neutral");
	}
}

void APatchStreamingManager::EstablishOrigin(const FIntPoint& Cell)
{
	OriginCell = Cell;

	if (bOverrideExitDirection)
	{
		ExitDirection = ExitDirectionOverride.GetSafeNormal();
	}

	if (ExitDirection.IsNearlyZero())
	{
		const FVector Forward3D = GetTrackedForward();
		const FVector2D Forward = FVector2D(Forward3D.X, Forward3D.Y).GetSafeNormal(UE_KINDA_SMALL_NUMBER);
		const double ForwardYaw = Forward.IsNearlyZero() ? 0.0 : FMath::RadiansToDegrees(FMath::Atan2(Forward.Y, Forward.X));

		// Pick a yaw offset outside the +/- MinExitAngleFromForward wedge around the player's forward.
		const double MinAngle = FMath::Clamp(static_cast<double>(MinExitAngleFromForward), 0.0, 179.0);
		const double Offset = FMath::RandRange(MinAngle, 360.0 - MinAngle);
		const double ExitYaw = FMath::DegreesToRadians(ForwardYaw + Offset);
		ExitDirection = FVector2D(FMath::Cos(ExitYaw), FMath::Sin(ExitYaw));
	}

	UE_LOG(LogPatchStreaming, Log, TEXT("%s: Origin cell (%d, %d), exit direction (%.2f, %.2f)."),
		*GetName(), Cell.X, Cell.Y, ExitDirection.X, ExitDirection.Y);
}

void APatchStreamingManager::UpdatePatches()
{
	FVector TrackedLocation;
	if (!GetTrackedLocation(TrackedLocation))
	{
		return;
	}

	const FIntPoint PlayerCell = WorldToCell(TrackedLocation);
	if (!OriginCell.IsSet())
	{
		EstablishOrigin(PlayerCell);
	}

	if (PlayerCell == LastPlayerCell && LoadedPatches.Num() >= FMath::Square(LoadRadius * 2 + 1))
	{
		// The ring was already requested. Keep polling visibility here as well so a
		// missed level-shown notification can't stall the initial-load broadcast.
		CheckInitialPatchesLoaded();
		if (bDrawDebug)
		{
			DrawDebug(PlayerCell);
		}
		return;
	}
	LastPlayerCell = PlayerCell;
	UpdateNavBounds(PlayerCell);

	TArray<FIntPoint> MissingCells;
	for (int32 DY = -LoadRadius; DY <= LoadRadius; ++DY)
	{
		for (int32 DX = -LoadRadius; DX <= LoadRadius; ++DX)
		{
			const FIntPoint Cell(PlayerCell.X + DX, PlayerCell.Y + DY);
			if (!LoadedPatches.Contains(Cell) && !FailedCells.Contains(Cell))
			{
				MissingCells.Add(Cell);
			}
		}
	}

	MissingCells.Sort([&PlayerCell](const FIntPoint& A, const FIntPoint& B)
	{
		return CellDistance(A, PlayerCell) < CellDistance(B, PlayerCell);
	});

	for (const FIntPoint& Cell : MissingCells)
	{
		while (LoadedPatches.Num() >= MaxPatches)
		{
			if (!EvictFarthest(PlayerCell))
			{
				break;
			}
		}
		LoadPatch(Cell);
	}

	CheckInitialPatchesLoaded();

	if (bDrawDebug)
	{
		DrawDebug(PlayerCell);
	}
}

bool APatchStreamingManager::GetTrackedLocation(FVector& OutLocation) const
{
	if (const APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		OutLocation = Pawn->GetActorLocation();
		return true;
	}

	if (InitialLoadLocation.IsSet())
	{
		OutLocation = InitialLoadLocation.GetValue();
		return true;
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		OutLocation = It->GetActorLocation();
		return true;
	}

	if (const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		OutLocation = Camera->GetCameraLocation();
		return true;
	}

	return false;
}

FVector APatchStreamingManager::GetTrackedForward() const
{
	if (const APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		return Pawn->GetActorForwardVector();
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		return It->GetActorForwardVector();
	}

	if (const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		return Camera->GetCameraRotation().Vector();
	}

	return FVector::ForwardVector;
}

void APatchStreamingManager::LoadPatch(const FIntPoint& Cell)
{
	const EPatchCategory Category = GetCellCategory(Cell);
	const int32 PatchIndex = SelectPatchIndex(Cell);
	if (PatchIndex == INDEX_NONE)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: No %s patches available for cell (%d, %d)."),
			*GetName(), CategoryToString(Category), Cell.X, Cell.Y);
		FailedCells.Add(Cell);
		return;
	}

	bool bSuccess = false;
	ULevelStreamingDynamic* Streaming = ULevelStreamingDynamic::LoadLevelInstance(
		GetWorld(), GetPool(Category)[PatchIndex].GetLongPackageName(), CellToWorld(Cell), FRotator::ZeroRotator, bSuccess);

	if (!bSuccess || !Streaming)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: Failed to load %s patch %d at cell (%d, %d)."),
			*GetName(), CategoryToString(Category), PatchIndex, Cell.X, Cell.Y);
		FailedCells.Add(Cell);
		return;
	}

	Streaming->OnLevelShown.AddDynamic(this, &APatchStreamingManager::HandleLevelShown);
	LoadedPatches.Add(Cell, Streaming);
	UE_LOG(LogPatchStreaming, Verbose, TEXT("%s: Loading %s patch %d at cell (%d, %d). Loaded count: %d"),
		*GetName(), CategoryToString(Category), PatchIndex, Cell.X, Cell.Y, LoadedPatches.Num());
}

void APatchStreamingManager::UnloadPatch(const FIntPoint& Cell)
{
	TObjectPtr<ULevelStreamingDynamic> Streaming;
	if (!LoadedPatches.RemoveAndCopyValue(Cell, Streaming))
	{
		return;
	}

	if (Streaming)
	{
		Streaming->OnLevelShown.RemoveDynamic(this, &APatchStreamingManager::HandleLevelShown);
		Streaming->SetShouldBeVisible(false);
		Streaming->SetShouldBeLoaded(false);
		Streaming->SetIsRequestingUnloadAndRemoval(true);
	}

	UE_LOG(LogPatchStreaming, Verbose, TEXT("%s: Unloading patch at cell (%d, %d). Loaded count: %d"),
		*GetName(), Cell.X, Cell.Y, LoadedPatches.Num());
}

bool APatchStreamingManager::EvictFarthest(const FIntPoint& PlayerCell)
{
	FIntPoint FarthestCell;
	int32 FarthestDistance = -1;

	for (const TPair<FIntPoint, TObjectPtr<ULevelStreamingDynamic>>& Pair : LoadedPatches)
	{
		const int32 Distance = CellDistance(Pair.Key, PlayerCell);
		if (Distance > FarthestDistance)
		{
			FarthestDistance = Distance;
			FarthestCell = Pair.Key;
		}
	}

	if (FarthestDistance <= LoadRadius)
	{
		return false;
	}

	UnloadPatch(FarthestCell);
	return true;
}

int32 APatchStreamingManager::CellDistance(const FIntPoint& A, const FIntPoint& B)
{
	return FMath::Max(FMath::Abs(A.X - B.X), FMath::Abs(A.Y - B.Y));
}

void APatchStreamingManager::HandleLevelShown()
{
	CheckInitialPatchesLoaded();
}

void APatchStreamingManager::CheckInitialPatchesLoaded()
{
	if (bInitialPatchesLoaded || LastPlayerCell.X == TNumericLimits<int32>::Max())
	{
		return;
	}

	for (int32 DY = -LoadRadius; DY <= LoadRadius; ++DY)
	{
		for (int32 DX = -LoadRadius; DX <= LoadRadius; ++DX)
		{
			const FIntPoint Cell(LastPlayerCell.X + DX, LastPlayerCell.Y + DY);
			if (FailedCells.Contains(Cell))
			{
				// A cell that cannot load should not stall the initial-load broadcast.
				continue;
			}
			const TObjectPtr<ULevelStreamingDynamic>* Streaming = LoadedPatches.Find(Cell);
			if (!Streaming || !*Streaming || !(*Streaming)->IsLevelVisible())
			{
				return;
			}
		}
	}

	bInitialPatchesLoaded = true;
	GetWorldTimerManager().ClearTimer(InitialLoadTimeoutHandle);
	UE_LOG(LogPatchStreaming, Log, TEXT("%s: Initial patch ring loaded around cell (%d, %d)."), *GetName(), LastPlayerCell.X, LastPlayerCell.Y);
	OnInitialPatchesLoaded.Broadcast();
}

void APatchStreamingManager::HandleInitialLoadTimeout()
{
	if (bInitialPatchesLoaded)
	{
		return;
	}

	// Report which ring cells never became visible to aid diagnosis.
	if (LastPlayerCell.X != TNumericLimits<int32>::Max())
	{
		for (int32 DY = -LoadRadius; DY <= LoadRadius; ++DY)
		{
			for (int32 DX = -LoadRadius; DX <= LoadRadius; ++DX)
			{
				const FIntPoint Cell(LastPlayerCell.X + DX, LastPlayerCell.Y + DY);
				const TObjectPtr<ULevelStreamingDynamic>* Streaming = LoadedPatches.Find(Cell);
				const bool bVisible = Streaming && *Streaming && (*Streaming)->IsLevelVisible();
				if (!bVisible)
				{
					UE_LOG(LogPatchStreaming, Warning,
						TEXT("%s: Patch at cell (%d, %d) not visible after %.1fs (level loaded: %s)."),
						*GetName(), Cell.X, Cell.Y, InitialLoadTimeout,
						(Streaming && *Streaming && (*Streaming)->IsLevelLoaded()) ? TEXT("yes") : TEXT("no"));
				}
			}
		}
	}

	UE_LOG(LogPatchStreaming, Warning, TEXT("%s: Initial patch ring incomplete; releasing waiting players anyway."), *GetName());
	bInitialPatchesLoaded = true;
	OnInitialPatchesLoaded.Broadcast();
}

void APatchStreamingManager::UpdateNavBounds(const FIntPoint& PlayerCell)
{
	if (!NavBoundsVolume)
	{
		return;
	}

	UBrushComponent* Brush = NavBoundsVolume->GetBrushComponent();
	if (!Brush)
	{
		return;
	}

	const FVector LocalSize = Brush->CalcBounds(FTransform::Identity).GetBox().GetSize();
	if (LocalSize.GetMin() <= UE_KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogPatchStreaming, Warning, TEXT("%s: Nav bounds volume %s has no brush geometry; cannot resize it."), *GetName(), *NavBoundsVolume->GetName());
		return;
	}

	const double RingSize = static_cast<double>(LoadRadius * 2 + 1) * PatchSize;
	const FVector Center(
		(PlayerCell.X + 0.5) * PatchSize,
		(PlayerCell.Y + 0.5) * PatchSize,
		PatchPivotOffset.Z);
	const FVector Scale(RingSize / LocalSize.X, RingSize / LocalSize.Y, NavBoundsHeight / LocalSize.Z);

	NavBoundsVolume->SetActorLocationAndRotation(Center, FRotator::ZeroRotator);
	NavBoundsVolume->SetActorScale3D(Scale);

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->OnNavigationBoundsUpdated(NavBoundsVolume);
	}
}

void APatchStreamingManager::DrawDebug(const FIntPoint& PlayerCell) const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector HalfExtent(PatchSize * 0.5, PatchSize * 0.5, 50.0);
	for (const TPair<FIntPoint, TObjectPtr<ULevelStreamingDynamic>>& Pair : LoadedPatches)
	{
		const FVector Center = FVector(Pair.Key.X * PatchSize, Pair.Key.Y * PatchSize, 0.0) + FVector(HalfExtent.X, HalfExtent.Y, 0.0);
		const bool bInRing = CellDistance(Pair.Key, PlayerCell) <= LoadRadius;
		const bool bLoaded = Pair.Value && Pair.Value->IsLevelLoaded();
		const FColor Color = !bLoaded ? FColor::Yellow : (bInRing ? FColor::Green : FColor::Red);
		const EPatchCategory Category = GetCellCategory(Pair.Key);
		DrawDebugBox(World, Center, HalfExtent, Color, false, PollInterval, 0, 50.0f);
		DrawDebugString(World, Center + FVector(0.0, 0.0, 200.0),
			FString::Printf(TEXT("(%d,%d) %s P%d"), Pair.Key.X, Pair.Key.Y, CategoryToString(Category), SelectPatchIndex(Pair.Key)),
			nullptr, Color, PollInterval);
	}

	if (OriginCell.IsSet())
	{
		const FVector OriginCenter = FVector(OriginCell->X * PatchSize, OriginCell->Y * PatchSize, 0.0) + FVector(HalfExtent.X, HalfExtent.Y, 200.0);
		const FVector ExitEnd = OriginCenter + FVector(ExitDirection.X, ExitDirection.Y, 0.0) * PatchSize;
		DrawDebugDirectionalArrow(World, OriginCenter, ExitEnd, PatchSize * 0.1f, FColor::Cyan, false, PollInterval, 0, 100.0f);
	}
}
