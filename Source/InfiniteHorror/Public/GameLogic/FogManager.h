// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogManager.generated.h"

class ACharacter;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

UCLASS()
class INFINITEHORROR_API AFogManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFogManager();

	/*
	 * @brief Reference to the Material Parameter Collection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialParameterCollection> MaterialParameterCollection;

	/*
	 * @brief Max squared distance the player can move before the fog re-centers
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog", meta = (AllowPrivateAccess = "true"))
	float MaxFogRecenterDistanceSqr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector PreviousPlayerFogPosition;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> MainCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInstance;
};

