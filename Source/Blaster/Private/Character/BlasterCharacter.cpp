// opyright Maxjestic.


#include "Character/BlasterCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/InputConfigDataAsset.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.f, 400.f, 0.f );

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	SpringArmComponent->SetupAttachment( GetMesh() );
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	CameraComponent->SetupAttachment( SpringArmComponent, USpringArmComponent::SocketName );
	CameraComponent->bUsePawnControlRotation = false;
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterCharacter::Move( const FInputActionValue& InputActionValue )
{
	if (!GetController())
	{
		return;
	}

	const FVector2D MoveVector = InputActionValue.Get<FVector2D>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator MovementRotation( 0.f, ControlRotation.Yaw, 0.f );

	if(MoveVector.Y)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput( ForwardDirection, MoveVector.Y );
	}

	if(MoveVector.X)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput( ForwardDirection, MoveVector.X );
	}
	
}

void ABlasterCharacter::Look( const FInputActionValue& InputActionValue )
{
	if (!GetController())
	{
		return;
	}
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput( LookAxisVector.X );
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput( LookAxisVector.Y );
	}
}

void ABlasterCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABlasterCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	if (const APlayerController* PlayerController = Cast<APlayerController>( GetController() ))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer() ))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext( InputMappingContext, 0 );
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( PlayerInputComponent ))
	{
		EnhancedInputComponent->BindAction( InputActions->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move );
		EnhancedInputComponent->BindAction( InputActions->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look );
		EnhancedInputComponent->BindAction( InputActions->JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump );
	}
}

void ABlasterCharacter::Jump()
{
	Super::Jump();
}
