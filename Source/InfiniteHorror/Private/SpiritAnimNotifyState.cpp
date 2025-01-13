// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "SpiritAnimNotifyState.h"

void USpiritAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        if (ANPC_Spirit* const Spirit = Cast<ANPC_Spirit>(MeshComp->GetOwner()))
        {
            Spirit->AttackStart();
        }
    }
}

void USpiritAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        if (ANPC_Spirit* const Spirit = Cast<ANPC_Spirit>(MeshComp->GetOwner()))
        {
            Spirit->AttackEnd();
        }
    }
}