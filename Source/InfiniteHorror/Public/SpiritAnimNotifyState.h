// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Spirit.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SpiritAnimNotifyState.generated.h"

/**
 * Event notifier for the spirit's animations
 */
UCLASS()
class INFINITEHORROR_API USpiritAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
