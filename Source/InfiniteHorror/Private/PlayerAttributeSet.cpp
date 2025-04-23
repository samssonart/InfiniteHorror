// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "PlayerAttributeSet.h"
#include "GameplayEffectExtension.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	// Check if the attribute that was modified is Stamina
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		
	}
	else if (Data.EvaluatedData.Attribute == GetEnduranceAttribute())
	{

	}
}
