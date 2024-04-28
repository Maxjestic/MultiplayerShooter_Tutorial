// Copyright Maxjestic.


#include "HUD/OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText( const FString& TextToDisplay ) const
{
	if (DisplayText)
	{
		DisplayText->SetText( FText::FromString( TextToDisplay ) );
	}
}

void UOverheadWidget::ShowPlayerAndNetRole( const APawn* InPawn ) const
{
	FString PlayerName;
	if(const APlayerState* PlayerState = InPawn->GetPlayerState())
	{
		PlayerName = PlayerState->GetPlayerName();
	}
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ROLE_Authority:
		Role = FString( "Authority" );
		break;
	case ROLE_SimulatedProxy:
		Role = FString( "SimulatedProxy" );
		break;
	case ROLE_AutonomousProxy:
		Role = FString( "AutonomousProxy" );
		break;
	default:
		Role = FString( "None" );
		break;
	}
	SetDisplayText( FString::Printf( TEXT("%s: %s."), *PlayerName ,*Role ) );
}
