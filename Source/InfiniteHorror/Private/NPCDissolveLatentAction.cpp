#include "NPCDissolveLatentAction.h"
#include "Kismet/GameplayStatics.h"


void NPCDissolveLatentAction::Initialize()
{
	_npcSpirit = Cast<ANPC_Spirit>(UGameplayStatics::GetActorOfClass(GEngine->GetWorld(), ANPC_Spirit::StaticClass()));
}

void NPCDissolveLatentAction::UpdateOperation(FLatentResponse& Response)
{
	_elapsedTime += _deltaTime;
	_transitionRatio = FMath::Clamp(_elapsedTime / _totalTime, 0.0f, 1.0f);

	if (_npcSpirit)
	{
		_npcSpirit->SetDissolveAmount(_transitionRatio);
	}

	Response.DoneIf(_elapsedTime >= _totalTime);
}