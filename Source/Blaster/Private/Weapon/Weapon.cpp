// Copyright Maxjestic.


#include "Weapon/Weapon.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

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

	if (HasAuthority())
	{
		AreaSphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
		AreaSphere->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
		AreaSphere->OnComponentBeginOverlap.AddDynamic( this, &ThisClass::OnSphereOverlap );
	}
	
	if(PickupWidget)
	{
		PickupWidget->SetVisibility( false );
	}
}

void AWeapon::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	const ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>( OtherActor );
	
	if(BlasterCharacter && PickupWidget)
	{
		PickupWidget->SetVisibility( true );
	}	
}

void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
