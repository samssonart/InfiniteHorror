// Copyright (c) 2024 Samssonart. All rights reserved.


#include "LightManager.h"
#include "Components/DirectionalLightComponent.h"

UDirectionalLightComponent* _directionalLightComponent;

// Sets default values
ALightManager::ALightManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALightManager::BeginPlay()
{
    Super::BeginPlay();

	if (MainLight->IsValidLowLevel()) 
	{
		_directionalLightComponent = 
			Cast<UDirectionalLightComponent>(MainLight->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
		if (_directionalLightComponent->IsValidLowLevel()) 
		{
			_directionalLightComponent->SetIntensity(GameIntensity);
		}
	}
}

void ALightManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (_directionalLightComponent->IsValidLowLevel())
	{
		_directionalLightComponent->SetIntensity(EditorIntensity);
	}
	UE_LOG(LogTemp, Warning, TEXT("EndPlay called with reason: %d"), EndPlayReason);
}

