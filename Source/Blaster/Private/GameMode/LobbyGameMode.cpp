// Copyright Maxjestic.


#include "GameMode/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin( APlayerController* NewPlayer )
{
	Super::PostLogin( NewPlayer );

	if(const int32 NumOfPlayers = GameState.Get()->PlayerArray.Num(); NumOfPlayers == 2)
	{
		if(UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel( FString("/Game/Maps/BlasterMap?listen") );
		}
	}
}
