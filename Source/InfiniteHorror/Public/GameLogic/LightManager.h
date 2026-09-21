// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightManager.generated.h"

class ADirectionalLight;

/**
 * @brief Manages the main directional light in the scene.
 */
UCLASS()
class INFINITEHORROR_API ALightManager : public AActor
{
    GENERATED_BODY()

public:
    /**
     * @brief Sets default values for this actor's properties.
     */
    ALightManager();

protected:
    /**
     * @brief Called when the game starts or when spawned.
     */
    virtual void BeginPlay() override;

    /** The main directional light in the scene. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    TObjectPtr<ADirectionalLight> MainLight;

    /** Intensity to use during gameplay. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    float GameIntensity = 10.0f;
};
