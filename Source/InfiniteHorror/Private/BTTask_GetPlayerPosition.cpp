// Copyright (c) 2024 Samssonart. All rights reserved.


#include "BTTask_GetPlayerPosition.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"

UBTTask_GetPlayerPosition::UBTTask_GetPlayerPosition(FObjectInitializer const& ObjecctInitializer)
{
	NodeName = "Finds Player actor and get its position";
}

EBTNodeResult::Type UBTTask_GetPlayerPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* GameWorld = GetWorld();
	APlayerController* const playerCont = UGameplayStatics::GetPlayerController(GetWorld(), 0);


	if (ACharacter* const PlayerActor = UGameplayStatics::GetPlayerCharacter(GameWorld, 0))
	{
		FVector PlayerLocation = PlayerActor->GetActorLocation();

		if (playerCont)
		{
			if (APlayerCameraManager* camMan = playerCont->PlayerCameraManager)
			{
				PlayerLocation += camMan->GetActorForwardVector() * PlayerOffset;
			}
		}
		else
		{
			PlayerLocation += PlayerActor->GetActorForwardVector() * PlayerOffset;
		}

		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		BBComp->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

