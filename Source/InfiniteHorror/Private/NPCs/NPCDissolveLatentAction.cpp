#include "NPCs/NPCDissolveLatentAction.h"
#include "NPCs/NPCSpirit.h"

void NPCDissolveLatentAction::Initialize()
{
	Spirit = Cast<ANPCSpirit>(NPCActor);
}

void NPCDissolveLatentAction::UpdateOperation(FLatentResponse& Response)
{
	ElapsedTime += Response.ElapsedTime();
	TransitionRatio = FMath::Clamp(ElapsedTime / TotalTime, 0.0f, 1.0f);

	if (Spirit)
	{
		Spirit->SetDissolveAmount(TransitionRatio);
	}

	Response.DoneIf(ElapsedTime >= TotalTime);
}