// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ABlasterCharacter;
class AWeapon;
class ABlasterCharacter;

/*
 * Component designated to handle combat related stuff
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UCombatComponent();

	//~ Begin UActorComponent Interface
	virtual void TickComponent( float DeltaTime, ELevelTick TickType,
	                            FActorComponentTickFunction* ThisTickFunction ) override;
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	//~ Begin UActorComponent Interface

	void EquipWeapon( AWeapon* const WeaponToEquip );
	void AimWeapon( const bool bIsAiming );

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	UFUNCTION( Server, Reliable )
	void ServerAim( const bool bIsAiming );

private:
	/** Owner of the component */
	TObjectPtr<ABlasterCharacter> Character;

	/** Currently equipped weapon */
	UPROPERTY( Replicated )
	TObjectPtr<AWeapon> EquippedWeapon;

	/** Aiming "state" */
	UPROPERTY( Replicated )
	bool bAiming = false;

	/*
	 * Friends
	 */
	friend ABlasterCharacter;
};
