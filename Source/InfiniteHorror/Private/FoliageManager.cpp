// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "FoliageManager.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedFoliage.h"

void AddFoliageInstancesCallback(const TArray<FDesiredFoliageInstance>& Instances)
{
    // Not sure what to do here
}

// Sets default values
AFoliageManager::AFoliageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoliageManager::BeginPlay()
{
	Super::BeginPlay();

    FoliageInfo = FFoliageInfo();

    if (ProceduralFoliageVolume->IsValidLowLevel())
    {
        ProceduralFoliageGenerator = ProceduralFoliageVolume->GetComponentByClass<UProceduralFoliageComponent>();
    }
    
    if (ProceduralFoliageGenerator)
    {
        FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
        PlayerPosition.Z = 820;
        ProceduralFoliageVolume->SetActorLocation(PlayerPosition);

        // Create a TFunctionRef from the function
        TFunctionRef<void(const TArray<FDesiredFoliageInstance>&)> FuncRef = 
            TFunctionRef<void(const TArray<FDesiredFoliageInstance>&)>(AddFoliageInstancesCallback);

        if (ProceduralFoliageGenerator->ResimulateProceduralFoliage(FuncRef))
        {
            UE_LOG(LogTemp, Log, TEXT("Foliage generation successful"));
        }
    }
}

// Called every frame
void AFoliageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

