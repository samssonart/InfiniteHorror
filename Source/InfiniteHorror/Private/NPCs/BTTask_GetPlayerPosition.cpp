// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/BTTask_GetPlayerPosition.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

UBTTask_GetPlayerPosition::UBTTask_GetPlayerPosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Finds Player actor and get its position";
}

EBTNodeResult::Type UBTTask_GetPlayerPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* GameWorld = GetWorld();
	APlayerController* const PlayerCont = UGameplayStatics::GetPlayerController(GameWorld, 0);

	if (ACharacter* const PlayerActor = UGameplayStatics::GetPlayerCharacter(GameWorld, 0))
	{
		FVector PlayerLocation = PlayerActor->GetActorLocation();

		if (PlayerCont)
		{
			if (APlayerCameraManager* CameraManager = PlayerCont->PlayerCameraManager)
			{
				PlayerLocation += CameraManager->GetActorForwardVector() * PlayerOffset;
			}
		}
		else
		{
			PlayerLocation += PlayerActor->GetActorForwardVector() * PlayerOffset;
		}

		UBlackboardComponent* const BBComp = OwnerComp.GetBlackboardComponent();
		if (!BBComp)
		{
			return EBTNodeResult::Failed;
		}
		BBComp->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


