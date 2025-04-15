// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CharacterState.generated.h"

class UDifficultyAbilitySystemComponent;

UCLASS()
class INFINITEHORROR_API ACharacterState : public APlayerState, public IAbilitySystemInterface
{
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Sets default values for this actor's properties
	ACharacterState();

private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
