// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * Game mode for lobby level
 */
UCLASS()
class BLASTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//~ Begin AGameModeBase Interface
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//~ End AGameModeBase Interface
	
};
