// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Spirit.h"
#include "LatentActions.h"

/**
 * @brief A basic latent action that will be executed for a specified amount of time.
 */
class INFINITEHORROR_API NPCDissolveLatentAction : public FPendingLatentAction
{
private:

	TObjectPtr<ANPC_Spirit> _npcSpirit;

protected:

	int _id;
	float _totalTime;
	float _elapsedTime;
	float& _deltaTime;
	float _transitionRatio = 0.0f;

public:

	NPCDissolveLatentAction(int id, float totalTime, float& deltaTime) : _id(id), _totalTime(totalTime), _deltaTime(deltaTime), _elapsedTime(0.0f) { Initialize(); };

	void Initialize();

	virtual void UpdateOperation(FLatentResponse& Response) override;
};