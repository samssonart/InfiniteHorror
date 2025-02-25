// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "NPCFactory.h"

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

	if (PlayerActor && NPCPaths.Num() > 0)
	{
		SpawnNPC(FMath::RandRange(0, NPCPaths.Num() - 1));
	}
	
}

void ANPCFactory::SpawnNPC(int NPCIndex)
{
	if (!bIsNPCInPlay)
	{
		FVector SpawnLocation = PlayerActor->GetActorLocation() + FVector(FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), FMath::RandRange(-NPCSpawnRadius, NPCSpawnRadius), 0.0f);
		FRotator SpawnRotation = PlayerActor->GetActorRotation();

		if (NPCPool.Num() >= NPCIndex)
		{
			NPCInPlay = NPCPool[NPCIndex];
		}
		else
		{
			NPCInPlay = SpawnNPCFromBlueprintPath(GetWorld(), NPCPaths[NPCIndex], SpawnLocation, SpawnRotation);
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

ANPC_Spirit* ANPCFactory::SpawnNPCFromBlueprintPath(UWorld* World, const FString BlueprintPath, const FVector& Location, const FRotator& Rotation)
{
	if (World)
	{

		TObjectPtr<UBlueprint> LoadedBlueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BlueprintPath));
		if (LoadedBlueprint && LoadedBlueprint->GeneratedClass)
		{
			FActorSpawnParameters SpawnParams;
			TObjectPtr<ANPC_Spirit> SpawnedNPC = Cast<ANPC_Spirit>(World->SpawnActor<AActor>(LoadedBlueprint->GeneratedClass, Location, Rotation, SpawnParams));
			if (SpawnedNPC)
			{
				NPCPool.AddUnique(SpawnedNPC);
				return SpawnedNPC;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load Blueprint at path: %s"), *BlueprintPath);
		}
	}

	return nullptr;
}

