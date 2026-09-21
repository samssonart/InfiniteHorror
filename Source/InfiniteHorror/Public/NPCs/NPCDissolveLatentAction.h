// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"

class ANPCSpirit;

/**
 * @brief A latent action that will gradually make an actor less visible for a specified amount of time.
 */
class INFINITEHORROR_API NPCDissolveLatentAction : public FPendingLatentAction
{
private:

	TObjectPtr<ANPCSpirit> Spirit;

protected:

	int32 ActionID;
	float TotalTime;
	float ElapsedTime;
	float TransitionRatio = 0.0f;
	TObjectPtr<AActor> NPCActor;

public:

	NPCDissolveLatentAction(int32 InActionID, float InTotalTime, AActor* ActorToDissolve)
	: ActionID(InActionID), TotalTime(InTotalTime), ElapsedTime(0.0f), NPCActor(ActorToDissolve)
	{
		Initialize();
	};

	void Initialize();

	virtual void UpdateOperation(FLatentResponse& Response) override;
};
