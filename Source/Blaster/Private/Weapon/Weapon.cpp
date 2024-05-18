// Copyright Maxjestic.


#include "Weapon/Weapon.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>( "WeaponMesh" );
	SetRootComponent( WeaponMesh );
	WeaponMesh->SetCollisionResponseToAllChannels( ECR_Block );
	WeaponMesh->SetCollisionResponseToChannel( ECC_Pawn, ECR_Ignore );
	WeaponMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	AreaSphere = CreateDefaultSubobject<USphereComponent>( "AreaSphere" );
	AreaSphere->SetupAttachment( RootComponent );
	AreaSphere->SetCollisionResponseToAllChannels( ECR_Ignore );
	AreaSphere->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>( "PickupWidget" );
	PickupWidget->SetupAttachment( RootComponent );
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() )
	{
		AreaSphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
		AreaSphere->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
		AreaSphere->OnComponentBeginOverlap.AddDynamic( this, &ThisClass::OnSphereBeginOverlap );
		AreaSphere->OnComponentEndOverlap.AddDynamic( this, &ThisClass::OnSphereEndOverlap );
	}

	if ( PickupWidget )
	{
		PickupWidget->SetVisibility( false );
	}
}

void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AWeapon::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AWeapon, WeaponState );
}

void AWeapon::OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>( OtherActor );

	if ( BlasterCharacter && PickupWidget )
	{
		BlasterCharacter->SetOverlappingWeapon( this );
	}
}

void AWeapon::OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex )
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>( OtherActor );

	if ( BlasterCharacter && PickupWidget )
	{
		BlasterCharacter->SetOverlappingWeapon( nullptr );
	}
}

void AWeapon::OnRep_WeaponState()
{
	SetWeaponState( WeaponState );
}

void AWeapon::ShowPickupWidget( const bool bShowWidget ) const
{
	if ( PickupWidget )
	{
		PickupWidget->SetVisibility( bShowWidget );
	}
}

void AWeapon::SetWeaponState( const EWeaponState State )
{
	WeaponState = State;	
	switch ( WeaponState )
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget( false );
		AreaSphere->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		break;
	default:
		break;
	}
}
