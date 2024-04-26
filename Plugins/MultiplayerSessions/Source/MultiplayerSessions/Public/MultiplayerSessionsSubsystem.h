// Copyright Maxjestic

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionsSubsystem.generated.h"

///
/// Declaration of our own custom delegates for the Menu class to bind callbacks to
///
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful );

DECLARE_MULTICAST_DELEGATE_TwoParams( FMultiplayerOnFindSessionsComplete,
                                      const TArray<FOnlineSessionSearchResult>& SessionResults,
                                      bool bWasSuccessful );

DECLARE_MULTICAST_DELEGATE_TwoParams( FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result, const FString& Address );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMultiplayerOnStartSessionComplete, bool, bWasSuccessful );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful );

/**
 * Handles sessions, working besides GameInstance
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * Binds internal callbacks to delegates
	 */
	UMultiplayerSessionsSubsystem();


	///
	/// To handle session functionality. The menu class will call these.
	///

	/**
	 * Accesses subsystem and calls its create session function
	 * 
	 * @param NumPublicConnections Number of players that can join the session
	 * @param MatchType Match type that will be handled by session
	 */
	void CreateSession( const int32 NumPublicConnections, FString MatchType );

	/**
	 * Accesses subsystem and calls its find sessions function
	 * 
	 * @param MaxSearchResults Search limit
	 */
	void FindSessions( const int32 MaxSearchResults );

	/**
	 * Accesses subsystem and calls its join session function
	 * 
	 * @param SessionSearchResult Found session to join
	 */
	void JoinSession( const FOnlineSessionSearchResult& SessionSearchResult );

	/** Accesses subsystem and calls its start function */
	void StartSession();

	/** Accesses subsystem and calls its destroy function */
	void DestroySession();

	///
	/// Our own custom delegates for the Menu class to bind callbacks to
	///
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

protected:
	///
	/// Internal callbacks for the delegates we'll add to the Online Session Interface list.
	/// These don't need to be called outside this class.
	///
	void OnCreateSessionComplete( FName SessionName, const bool bWasSuccessful );
	void OnFindSessionsComplete( const bool bWasSuccessful );
	void OnJoinSessionComplete( FName SessionName, EOnJoinSessionCompleteResult::Type Result );
	void OnStartSessionComplete( FName SessionName, const bool bWasSuccessful );
	void OnDestroySessionComplete( FName SessionName, const bool bWasSuccessful );

private:
	/** Check if session interface is initialized and valid */
	bool IsValidSessionInterface();
	
	/** Used to handle sessions */
	IOnlineSessionPtr SessionInterface;

	/** Session setting created in most recent session creation */
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	/** Session search created in most recent session finding */
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	///
	/// To add to Online Session Interface delegate list.
	/// We'll bind our MultiplayerSessionSubsystem internal callbacks to these.
	///
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

	///
	/// Delegate handles for delegates above
	///
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	/** Indicates if new session should be created after destroying one */
	bool bCreateSessionOnDestroy = false;

	/** Most recent number of public connections used to create session that failed */
	int32 LastNumPublicConnections = 4;

	/** Most recent match type used to create session that failed */
	FString LastMatchType;
};
