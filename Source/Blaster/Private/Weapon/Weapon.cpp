// Copyright Maxjestic.


#include "Weapon/Weapon.h"

#include "Components/SphereComponent.h"

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
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AreaSphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
		AreaSphere->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	}
}

void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
