// Copyright Maxjestic


#include "Menu.h"

#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"

void UMenu::MenuSetup( const int32 InNumOfPublicConnections, const FString InMatchType, const FString InPathToLobby )
{
	PathToLobby = FString::Printf( TEXT( "%s?listen" ), *InPathToLobby );
	NumPublicConnections = InNumOfPublicConnections;
	MatchType = InMatchType;

	AddToViewport();
	SetVisibility( ESlateVisibility::Visible );
	SetIsFocusable( true );

	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus( TakeWidget() );
			InputModeData.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
			PlayerController->SetInputMode( InputModeData );
			PlayerController->SetShowMouseCursor( true );
		}
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic( this, &ThisClass::OnCreateSession );
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject( this, &ThisClass::OnFindSessions );
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject( this, &ThisClass::OnJoinSession );
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic( this, &ThisClass::OnStartSession );
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic( this, &ThisClass::OnDestroySession );
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Button_Host)
	{
		Button_Host->OnClicked.AddDynamic( this, &ThisClass::HostButtonClicked );
	}

	if (Button_Join)
	{
		Button_Join->OnClicked.AddDynamic( this, &ThisClass::JoinButtonClicked );
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

// ReSharper disable once CppMemberFunctionMayBeConst, Callback to dynamic delegate
void UMenu::OnCreateSession( const bool bWasSuccessful )
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Yellow, FString( TEXT( "Session created successfully!" ) ) );
		}

		if (UWorld* World = GetWorld())
		{
			World->ServerTravel( PathToLobby );
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString( TEXT( "Failed to create session!" ) ) );
		}
		Button_Host->SetIsEnabled( true );
	}
}

void UMenu::OnFindSessions( const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful ) const
{
	if (!MultiplayerSessionsSubsystem)
	{
		return;
	}

	for (FOnlineSessionSearchResult Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get( FName( "MatchType" ), SettingsValue );
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession( Result );
			return;
		}
	}

	Button_Join->SetIsEnabled( true );
}

void UMenu::OnJoinSession( const EOnJoinSessionCompleteResult::Type Result, const FString& Address ) const
{
	if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel( Address, TRAVEL_Absolute );
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		Button_Join->SetIsEnabled( true );
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic, Callback to dynamic delegate
void UMenu::OnStartSession( const bool bWasSuccessful )
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString( TEXT( "Session started successfully!" ) ) );
		}
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString( TEXT( "Failed to start session!" ) ) );
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic, Callback to dynamic delegate
void UMenu::OnDestroySession( const bool bWasSuccessful )
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString( TEXT( "Session destroyed successfully!" ) ) );
		}
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString( TEXT( "Failed to destroy session!" ) ) );
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst, Callback to dynamic delegate
void UMenu::HostButtonClicked()
{
	Button_Host->SetIsEnabled( false );
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession( NumPublicConnections, MatchType );
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst, Callback to dynamic delegate
void UMenu::JoinButtonClicked()
{
	Button_Join->SetIsEnabled( false );
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions( 10000 );
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();

	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode( InputModeData );
			PlayerController->SetShowMouseCursor( false );
		}
	}
}
