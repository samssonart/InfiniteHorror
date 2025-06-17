// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#include "LightManager.h"
#include "Components/DirectionalLightComponent.h"

ALightManager::ALightManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ALightManager::BeginPlay()
{
    Super::BeginPlay();

    if (MainLight && MainLight->IsValidLowLevel())
    {
        DirectionalLightComponent =
            Cast<UDirectionalLightComponent>(MainLight->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
        if (DirectionalLightComponent && DirectionalLightComponent->IsValidLowLevel())
        {
            DirectionalLightComponent->SetIntensity(GameIntensity);
        }
    }
}

void ALightManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (DirectionalLightComponent && DirectionalLightComponent->IsValidLowLevel())
    {
        DirectionalLightComponent->SetIntensity(EditorIntensity);
    }
    UE_LOG(LogTemp, Warning, TEXT("EndPlay called with reason: %d"), EndPlayReason);
}