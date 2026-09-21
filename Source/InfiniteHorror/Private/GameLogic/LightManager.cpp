// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#include "GameLogic/LightManager.h"
#include "InfiniteHorrorLog.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

ALightManager::ALightManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALightManager::BeginPlay()
{
	Super::BeginPlay();

	if (MainLight)
	{
		if (UDirectionalLightComponent* LightComponent = MainLight->GetComponent())
		{
			LightComponent->SetIntensity(GameIntensity);
		}
		else
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: MainLight has no directional light component."), *GetName());
		}
	}
}
