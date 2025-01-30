// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralFoliageComponent.h"
#include "ProceduralFoliageVolume.h"
#include "FoliageManager.generated.h"

UCLASS()
class INFINITEHORROR_API AFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageManager();

	/*
	* @brief The actual PFG component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PFG")
	TObjectPtr<UProceduralFoliageComponent> ProceduralFoliageGenerator;

	/*
	* @brief The actor that contains the procedural foliage volume
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PFG")
	TObjectPtr<AActor> ProceduralFoliageVolume;

	/*
	* @brief The foliage info
	*/
	FFoliageInfo FoliageInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
