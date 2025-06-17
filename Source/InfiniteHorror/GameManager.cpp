// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#include "GameManager.h"
#include "Engine/Engine.h"

TObjectPtr<UGameManager> UGameManager::Instance = nullptr;

TObjectPtr<UGameManager> UGameManager::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UGameManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void UGameManager::Init()
{
}
