// Copyright Maxjestic.

#include "Component/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Weapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent( float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UCombatComponent::EquipWeapon( AWeapon* const WeaponToEquip )
{
	if ( Character == nullptr || WeaponToEquip == nullptr )
	{
		return;		
	}
	
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState( EWeaponState::EWS_Equipped );
	if ( const USkeletalMeshSocket* HandSocket = Character->GetMesh()->
															GetSocketByName( FName( "RightHandSocket" ) ) )
	{
		HandSocket->AttachActor( EquippedWeapon, Character->GetMesh() );
	}
	EquippedWeapon->SetOwner( Character );
}
