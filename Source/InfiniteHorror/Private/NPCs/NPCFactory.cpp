// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#include "NPCs/NPCFactory.h"
#include "NPCs/NPCSpirit.h"
#include "Player/PlayerCharacter.h"
#include "World/PatchStreamingManager.h"
#include "InfiniteHorrorLog.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ANPCFactory::ANPCFactory()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCFactory::ResetSpawnState()
{
	bIsNPCInPlay = false;
	NPCRespawnTimer = FMath::RandRange(1.0f,NPCReSpawnDelay);
}

void ANPCFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bTerrainReady)
	{
		return;
	}

	if (!PlayerActor)
	{
		PlayerActor = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if (!bIsNPCInPlay)
	{
		NPCRespawnTimer -= DeltaTime;
		if (NPCRespawnTimer <= 0 && PlayerActor && NPCRefs.Num() > 0)
		{
			SpawnNPC(FMath::RandRange(0, NPCRefs.Num() - 1));
		}
	}
}

// Called when the game starts
void ANPCFactory::BeginPlay()
{
	Super::BeginPlay();

	APatchStreamingManager* PatchManager = Cast<APatchStreamingManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APatchStreamingManager::StaticClass()));
	if (!PatchManager || PatchManager->AreInitialPatchesLoaded())
	{
		OnInitialPatchesLoaded();
		return;
	}

	PatchManager->OnInitialPatchesLoaded.AddUniqueDynamic(this, &ANPCFactory::OnInitialPatchesLoaded);
}

void ANPCFactory::OnInitialPatchesLoaded()
{
	bTerrainReady = true;
	PlayerActor = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerActor && NPCRefs.Num() > 0)
	{
		SpawnNPC(FMath::RandRange(0, NPCRefs.Num() - 1));
	}
}

void ANPCFactory::SpawnNPC(int NPCIndex)
{
	if (!bIsNPCInPlay && PlayerActor && NPCRefs.IsValidIndex(NPCIndex))
	{
		FVector SpawnLocation = PlayerActor->GetActorLocation() +
			FVector(FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), 0.0f);
		// Get the terrain height at the new position
		float TerrainHeight = 0.0f;
		FHitResult HitResult;

		// Perform a line trace to get the terrain height
		FVector TraceStart = SpawnLocation + FVector(0.0f, 0.0f, SpawnTraceHalfHeight); 
		FVector TraceEnd = SpawnLocation - FVector(0.0f, 0.0f, SpawnTraceHalfHeight); 

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
		{
			TerrainHeight = HitResult.Location.Z;
		}

		// Set the Z position just above the terrain height
		SpawnLocation.Z = TerrainHeight + NPCHeightOffset; 
		FRotator SpawnRotation = PlayerActor->GetActorRotation();

		NPCInPlay = SpawnNPCFromSubclassRef(GetWorld(), NPCIndex, SpawnLocation, SpawnRotation);
		
		if (NPCInPlay)
		{
			NPCInPlay->SetActorLocation(SpawnLocation);
			SpawnRotation.Yaw += FMath::RandRange(-180.0f, 180.0f);
			NPCInPlay->SetActorRotation(SpawnRotation);
			NPCInPlay->SetActorEnableCollision(true);
			bIsNPCInPlay = true;
		}
	}
}

ANPCSpirit* ANPCFactory::SpawnNPCFromSubclassRef(UWorld* World, const int NPCIndex, const FVector& Location, const FRotator& Rotation)
{
	if (World && NPCRefs.IsValidIndex(NPCIndex) && NPCRefs[NPCIndex])
	{
		FActorSpawnParameters SpawnParams;
		TObjectPtr<ANPCSpirit> SpawnedNPC = Cast<ANPCSpirit>
		(World->SpawnActor<ANPCSpirit>(NPCRefs[NPCIndex], Location, Rotation, SpawnParams));
		if (SpawnedNPC)
		{
			return SpawnedNPC;
		}
		else
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("Failed to load Blueprint at index: %d"), NPCIndex);
		}
	}

	return nullptr;
}


