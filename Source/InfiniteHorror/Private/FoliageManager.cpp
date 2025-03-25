// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "FoliageManager.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedFoliage.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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

    _mainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (_mainCharacter)
    {
        _previousPlayerFogPosition = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
    }
    if (MaterialParameterCollection)
    {
        _MPC_Instance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
    }

    if(_MPC_Instance)
    {
        _MPC_Instance->SetVectorParameterValue(FName("FogCenter"), _previousPlayerFogPosition);
    }
}

// Called every frame
void AFoliageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (_mainCharacter && _MPC_Instance)
    {
        _fogSqrDisplacement = FVector::DistSquared(_mainCharacter->GetActorLocation(), _previousPlayerFogPosition);
        if (_fogSqrDisplacement > MaxFogRecenterDistanceSqr)
        {
            _previousPlayerFogPosition = _mainCharacter->GetActorLocation();
            _MPC_Instance->SetVectorParameterValue(FName("FogCenter"), _previousPlayerFogPosition);
        }
    }
}

