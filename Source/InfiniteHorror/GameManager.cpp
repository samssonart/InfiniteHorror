// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameManager.h"
#include <string>
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UGameManager* UGameManager::_instance = nullptr;

UGameManager* UGameManager::GetInstance()
{
	if (!_instance)
	{
		_instance = NewObject<UGameManager>();
		_instance->AddToRoot();
	}
	return _instance;
}


void UGameManager::Init()
{
}

