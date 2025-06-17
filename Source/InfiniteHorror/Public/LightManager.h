// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightManager.generated.h"

class UDirectionalLightComponent;
class UActorComponent;

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

    /**
     * @brief Called when the actor is being removed from the level.
     * @param EndPlayReason The reason the actor is being removed.
     */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /** The main light actor in the scene. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    TObjectPtr<AActor> MainLight;

    /** Intensity to use during gameplay. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    float GameIntensity = 10.0f;

    /** Intensity to use in the editor. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
    float EditorIntensity = 1.0f;

private:
    /** Cached reference to the directional light component. */
    TObjectPtr<UDirectionalLightComponent> DirectionalLightComponent = nullptr;
};