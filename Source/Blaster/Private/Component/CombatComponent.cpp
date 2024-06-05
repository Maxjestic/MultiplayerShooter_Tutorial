// Copyright Maxjestic.

#include "Component/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::OnRep_EquippedWeapon() const
{
	if(EquippedWeapon && Character)
	{		
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::TickComponent( float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UCombatComponent::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( UCombatComponent, EquippedWeapon )
	DOREPLIFETIME( UCombatComponent, bAiming )
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
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::AimWeapon( const bool bIsAiming )
{
	bAiming = bIsAiming;
	ServerAim( bIsAiming );
}

void UCombatComponent::ServerAim_Implementation( const bool bIsAiming )
{
	bAiming = bIsAiming;
}
