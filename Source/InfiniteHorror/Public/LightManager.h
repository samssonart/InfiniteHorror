// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Light.h"
#include "LightManager.generated.h"

UCLASS()
class INFINITEHORROR_API ALightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightManager();

	/*
	* @brief Reference to the scene's main light
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Light Intensity")
	TObjectPtr<ALight> MainLight;

	/*
	* @brief The light intensity for gameplay
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Light Intensity")
	float GameIntensity;

	/*
	* @brief The light intensity for the editor. Gameplay is usually a very low value that makes it hard to see in the editor
	* @see GameIntensity
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Light Intensity")
	float EditorIntensity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
