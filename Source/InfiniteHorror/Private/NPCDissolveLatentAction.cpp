#include "NPCDissolveLatentAction.h"

void NPCDissolveLatentAction::Initialize()
{
	_npcSpirit = Cast<ANPC_Spirit>(_NPCActor);
}

void NPCDissolveLatentAction::UpdateOperation(FLatentResponse& Response)
{
	_elapsedTime += _deltaTime;
	_transitionRatio = FMath::Clamp(_elapsedTime / _totalTime, 0.0f, 1.0f);

	/*GEngine->AddOnScreenDebugMessage(-1, _deltaTime, FColor::Yellow,
		FString::Printf(TEXT("Dissolvetime %f"), _elapsedTime));*/

	if (_npcSpirit)
	{
		_npcSpirit->SetDissolveAmount(_transitionRatio);
	}

	Response.DoneIf(_elapsedTime >= _totalTime);
}