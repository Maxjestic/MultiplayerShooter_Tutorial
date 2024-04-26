// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;

/**
 * Simple menu connected to multiplayer session subsystem
 * Allows to
 *	host a game with specified number of players and match type
 *	join a hosted game
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Adds widget to viewport, sets properties */
	UFUNCTION( BlueprintCallable )
	void MenuSetup( const int32 InNumOfPublicConnections = 4, const FString InMatchType = FString( TEXT( "FreeForAll" ) ),
	                const FString InPathToLobby = FString( TEXT( "/Game/ThirdPerson/Maps/Lobby" ) ) );

protected:
	//~ Begin UUserWidget Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

	///
	/// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	///
	UFUNCTION()
	void OnCreateSession( const bool bWasSuccessful );
	
	void OnFindSessions( const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful ) const;
	void OnJoinSession( const EOnJoinSessionCompleteResult::Type Result, const FString& Address ) const;

	UFUNCTION()
	void OnStartSession( const bool bWasSuccessful );

	UFUNCTION()
	void OnDestroySession( const bool bWasSuccessful );

private:
	///
	/// Callbacks to buttons events
	///
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();


	/** Removes widget and takes care of input settings */
	void MenuTearDown();

	///
	/// Pointers to buttons in widget used to bind to events
	///
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UButton> Button_Host;

	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UButton> Button_Join;

	/** Handles online session functionality */
	TObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	/** Number of maximum player connections when creating an online session */
	int32 NumPublicConnections = 4;

	/** Match type for a session */
	FString MatchType = TEXT( "FreeForAll" );

	/** Path to lobby map file  */
	FString PathToLobby{ TEXT( "" ) };
};
