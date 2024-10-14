// Copyright (c) 2024 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProceduralFoliageComponent.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:

	static UGameManager* GetInstance();
	virtual void Init() override;
	UProceduralFoliageComponent* ProceduralFoliageGenerator;
	AActor* ProceduralFoliageVolume;

private:
	static UGameManager* _instance;
};
