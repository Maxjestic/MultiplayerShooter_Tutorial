// Copyright Maxjestic.


#include "Character/BlasterPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/BlasterCharacter.h"
#include "Input/InputConfigDataAsset.h"

ABlasterPlayerController::ABlasterPlayerController()
{
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if ( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer() ) )
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext( InputMappingContext, 0 );
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( InputComponent );

	EnhancedInputComponent->BindAction( InputActions->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move );
	EnhancedInputComponent->BindAction( InputActions->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look );
	EnhancedInputComponent->BindAction( InputActions->JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump );
	EnhancedInputComponent->BindAction( InputActions->EquipAction, ETriggerEvent::Triggered, this, &ThisClass::Equip );
	EnhancedInputComponent->BindAction( InputActions->CrouchAction,
	                                    ETriggerEvent::Triggered,
	                                    this,
	                                    &ThisClass::Crouch );
}

void ABlasterPlayerController::Move( const FInputActionValue& InputActionValue )
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if ( MovementVector.IsZero() )
	{
		return;
	}

	if ( APawn* ControlledPawn = GetPawn() )
	{
		const FRotator PawnRotation = ControlledPawn->GetControlRotation();
		const FRotator YawRotation( 0.f, PawnRotation.Yaw, 0.f );

		const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		ControlledPawn->AddMovementInput( ForwardDirection, MovementVector.Y );

		const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
		ControlledPawn->AddMovementInput( RightDirection, MovementVector.X );
	}
}

void ABlasterPlayerController::Look( const FInputActionValue& InputActionValue )
{
	if ( APawn* ControlledPawn = GetPawn() )
	{
		const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
		ControlledPawn->AddControllerYawInput( LookAxisVector.X );
		ControlledPawn->AddControllerPitchInput( LookAxisVector.Y );
	}
}

void ABlasterPlayerController::Jump()
{
	if ( GetCharacter() )
	{
		GetCharacter()->Jump();
	}
}

void ABlasterPlayerController::Equip()
{
	if ( const ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>( GetCharacter() ) )
	{
		BlasterCharacter->EquipWeapon();
	}
}

void ABlasterPlayerController::Crouch()
{
	if ( const ACharacter* BlasterCharacter = GetCharacter() )
	{
		if ( BlasterCharacter->bIsCrouched )
		{
			GetCharacter()->UnCrouch();
		}
		else
		{
			GetCharacter()->Crouch();
		}
	}
}
