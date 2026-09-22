// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "GameLogic/FogManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MainCharacter)
	{
		PreviousPlayerFogPosition = MainCharacter->GetActorLocation();
	}
	if (MaterialParameterCollection)
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	}

	if (MPCInstance)
	{
		MPCInstance->SetVectorParameterValue(FName("FogCenter"), PreviousPlayerFogPosition);
	}
}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!MainCharacter)
	{
		MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (MainCharacter)
		{
			PreviousPlayerFogPosition = MainCharacter->GetActorLocation();
		}
	}
	if (MainCharacter && MPCInstance)
	{
		const FVector PlayerPosition = MainCharacter->GetActorLocation();
		const float FogSqrDisplacement = FVector::DistSquared(PlayerPosition, PreviousPlayerFogPosition);
		if (FogSqrDisplacement > MaxFogRecenterDistanceSqr)
		{
			PreviousPlayerFogPosition = PlayerPosition;
			MPCInstance->SetVectorParameterValue(FName("FogCenter"), PreviousPlayerFogPosition);
		}
	}
}


