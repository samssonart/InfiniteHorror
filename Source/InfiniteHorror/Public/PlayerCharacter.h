// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Character.h"
#include "UIWidgetController.h"
#include "PlayerCharacter.generated.h"

/**
 * The player character class
 */
UCLASS()
class INFINITEHORROR_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetCurrentMentalHealth() const { return MentalHealth; }
	float GetCurrentBatteryLevel() const { return BatteryLevel; }
	void SetMentalHealth(float const NewHealth) { MentalHealth = NewHealth; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimuli();
	void ToggleTorch();

	UUIWidgetController* WidgetController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsTorchOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float MentalHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryLevel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryDepletionRate = 0.15f;

};
