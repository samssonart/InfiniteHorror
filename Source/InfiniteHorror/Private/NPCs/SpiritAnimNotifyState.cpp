// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/SpiritAnimNotifyState.h"
#include "NPCs/NPCSpirit.h"

void USpiritAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        if (ANPCSpirit* const Spirit = Cast<ANPCSpirit>(MeshComp->GetOwner()))
        {
            Spirit->AttackStart();
        }
    }
}

void USpiritAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        if (ANPCSpirit* const Spirit = Cast<ANPCSpirit>(MeshComp->GetOwner()))
        {
            Spirit->AttackEnd();
        }
    }
}
