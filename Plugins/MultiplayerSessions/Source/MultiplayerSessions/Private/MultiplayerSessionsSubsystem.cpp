// Copyright Maxjestic


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
	: CreateSessionCompleteDelegate( FOnCreateSessionCompleteDelegate::CreateUObject( this, &ThisClass::OnCreateSessionComplete ) ),
	  FindSessionsCompleteDelegate( FOnFindSessionsCompleteDelegate::CreateUObject( this, &ThisClass::OnFindSessionsComplete ) ),
	  JoinSessionCompleteDelegate( FOnJoinSessionCompleteDelegate::CreateUObject( this, &ThisClass::OnJoinSessionComplete ) ),
	  StartSessionCompleteDelegate( FOnStartSessionCompleteDelegate::CreateUObject( this, &ThisClass::OnStartSessionComplete ) ),
	  DestroySessionCompleteDelegate( FOnStartSessionCompleteDelegate::CreateUObject( this, &ThisClass::OnDestroySessionComplete ) )
{
	//if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	//{
	//	SessionInterface = Subsystem->GetSessionInterface();
	//}
}

void UMultiplayerSessionsSubsystem::CreateSession( const int32 NumPublicConnections, FString MatchType )
{
	if (!IsValidSessionInterface())
	{
		return;
	}

	if (SessionInterface->GetNamedSession( NAME_GameSession ))
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle( CreateSessionCompleteDelegate );

	LastSessionSettings = MakeShareable( new FOnlineSessionSettings() );
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set( FName( "MatchType" ), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	LastSessionSettings->BuildUniqueId = 1;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession( *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings ))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle( CreateSessionCompleteDelegateHandle );
		MultiplayerOnCreateSessionComplete.Broadcast( false );
	}
}

void UMultiplayerSessionsSubsystem::FindSessions( const int32 MaxSearchResults )
{
	if (!IsValidSessionInterface())
	{
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle( FindSessionsCompleteDelegate );

	LastSessionSearch = MakeShareable( new FOnlineSessionSearch() );
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set( SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals );

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions( *LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef() ))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle( FindSessionsCompleteDelegateHandle );
		MultiplayerOnFindSessionsComplete.Broadcast( TArray<FOnlineSessionSearchResult>(), false );
	}
}

void UMultiplayerSessionsSubsystem::JoinSession( const FOnlineSessionSearchResult& SessionSearchResult )
{
	if (!IsValidSessionInterface())
	{
		MultiplayerOnJoinSessionComplete.Broadcast( EOnJoinSessionCompleteResult::UnknownError, FString( "" ) );
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle( JoinSessionCompleteDelegate );

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession( *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSearchResult ))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle( JoinSessionCompleteDelegateHandle );
		MultiplayerOnJoinSessionComplete.Broadcast( EOnJoinSessionCompleteResult::UnknownError, FString( TEXT( "" ) ) );
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
	if (!IsValidSessionInterface())
	{
		return;
	}

	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle( StartSessionCompleteDelegate );

	if (!SessionInterface->StartSession( NAME_GameSession ))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle( StartSessionCompleteDelegateHandle );
		MultiplayerOnStartSessionComplete.Broadcast( false );
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!IsValidSessionInterface())
	{
		MultiplayerOnDestroySessionComplete.Broadcast( false );
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle( DestroySessionCompleteDelegate );
	if (!SessionInterface->DestroySession( NAME_GameSession ))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle( DestroySessionCompleteDelegateHandle );
		MultiplayerOnDestroySessionComplete.Broadcast( false );
	}
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete( FName SessionName, const bool bWasSuccessful )
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle( CreateSessionCompleteDelegateHandle );
	}

	MultiplayerOnCreateSessionComplete.Broadcast( bWasSuccessful );
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete( const bool bWasSuccessful )
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle( FindSessionsCompleteDelegateHandle );
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionsComplete.Broadcast( TArray<FOnlineSessionSearchResult>(), false );
		return;
	}

	MultiplayerOnFindSessionsComplete.Broadcast( LastSessionSearch->SearchResults, bWasSuccessful );
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete( FName SessionName, EOnJoinSessionCompleteResult::Type Result )
{
	FString Address{ TEXT( "" ) };
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle( JoinSessionCompleteDelegateHandle );
		SessionInterface->GetResolvedConnectString( NAME_GameSession, Address );
	}

	MultiplayerOnJoinSessionComplete.Broadcast( Result, Address );
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete( FName SessionName, const bool bWasSuccessful )
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle( StartSessionCompleteDelegateHandle );
	}

	MultiplayerOnStartSessionComplete.Broadcast( bWasSuccessful );
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete( FName SessionName, const bool bWasSuccessful )
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle( DestroySessionCompleteDelegateHandle );
	}

	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession( LastNumPublicConnections, LastMatchType );
	}

	MultiplayerOnDestroySessionComplete.Broadcast( bWasSuccessful );
}

bool UMultiplayerSessionsSubsystem::IsValidSessionInterface()
{
	if (!SessionInterface)
	{
		if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
		{
			SessionInterface = Subsystem->GetSessionInterface();
		}
	}
	return SessionInterface.IsValid();
}
