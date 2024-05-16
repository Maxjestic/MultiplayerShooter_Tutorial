// opyright Maxjestic.


#include "Character/BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"

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

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>( TEXT( "OverheadWidget" ) );
	OverheadWidget->SetupAttachment( GetRootComponent() );
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABlasterCharacter::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME_CONDITION( ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly );
}

void ABlasterCharacter::SetOverlappingWeapon( AWeapon* InOverlappingWeapon )
{
	if (IsLocallyControlled() && OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget( false );
	}

	OverlappingWeapon = InOverlappingWeapon;
	if (IsLocallyControlled() && OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget( true );
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon( AWeapon* LastWeapon ) const
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget( true );
	}

	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget( false );
	}
}
