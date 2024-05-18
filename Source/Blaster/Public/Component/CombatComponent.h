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
	
	//~ Begin AActorComponent Interface
	virtual void TickComponent( float DeltaTime, ELevelTick TickType,
								FActorComponentTickFunction* ThisTickFunction ) override;
	//~ Begin AActorComponent Interface

	void EquipWeapon( AWeapon* const WeaponToEquip);

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	/** Owner of the component */
	TObjectPtr<ABlasterCharacter> Character;
	
	/** Currently equipped weapon */
	TObjectPtr<AWeapon> EquippedWeapon;

	/*
	 * Friends
	 */
	friend ABlasterCharacter;
};
