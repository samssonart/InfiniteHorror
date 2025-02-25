// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#include "NPCFactory.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "NPC_Spirit_AIController.h"

// Sets default values
ANPCFactory::ANPCFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts
void ANPCFactory::BeginPlay()
{
	PlayerActor = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerActor && NPCRefs.Num() > 0)
	{
		SpawnNPC(FMath::RandRange(0, NPCRefs.Num() - 1));
	}
	
}

void ANPCFactory::SpawnNPC(int NPCIndex)
{
	if (!bIsNPCInPlay)
	{
		FVector SpawnLocation = PlayerActor->GetActorLocation() +
			FVector(FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), 0.0f);
		// Get the terrain height at the new position
		float TerrainHeight = 0.0f;
		FHitResult HitResult;

		// Perform a line trace to get the terrain height
		FVector TraceStart = SpawnLocation + FVector(0.0f, 0.0f, NPCSpawnRadius); 
		FVector TraceEnd = SpawnLocation - FVector(0.0f, 0.0f, NPCSpawnRadius); 

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
		{
			TerrainHeight = HitResult.Location.Z;
		}

		// Set the Z position just above the terrain height
		SpawnLocation.Z = TerrainHeight + NPCHeightOffset; 
		FRotator SpawnRotation = PlayerActor->GetActorRotation();

		if (!NPCPool.IsEmpty() && NPCPool.Num() >= NPCIndex)
		{
			NPCInPlay = NPCPool[NPCIndex];
		}
		else
		{
			NPCInPlay = SpawnNPCFromSubclassRef(GetWorld(), NPCIndex, SpawnLocation, SpawnRotation);
		}
		
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

ANPC_Spirit* ANPCFactory::SpawnNPCFromSubclassRef(UWorld* World, const int NPCIndex, const FVector& Location, const FRotator& Rotation)
{
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		TObjectPtr<ANPC_Spirit> SpawnedNPC = Cast<ANPC_Spirit>
		(World->SpawnActor<ANPC_Spirit>(NPCRefs[NPCIndex], Location, Rotation, SpawnParams));
		if (SpawnedNPC )
		{
			NPCPool.AddUnique(SpawnedNPC);
			return SpawnedNPC;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load Blueprint at index: %d"), NPCIndex);
		}
	}

	return nullptr;
}

