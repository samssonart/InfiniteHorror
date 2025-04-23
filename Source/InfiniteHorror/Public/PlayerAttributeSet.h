// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
	

/*
 * Player attributes that are subject to change depending on difficulty.
 */
UCLASS()
class INFINITEHORROR_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Stamina);

	UPROPERTY(BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Endurance);
};
