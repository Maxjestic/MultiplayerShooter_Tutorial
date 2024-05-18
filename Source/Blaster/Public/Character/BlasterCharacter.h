// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * Main character class in Blaster
 */
UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * Sets up: Camera with spring arm, overhead widget
	 */
	ABlasterCharacter();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	//~ End AActor Interface

	//~ Begin ACharacter Interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void PostInitializeComponents() override;
	//~ BEnd ACharacter Interface

	/** Setter for overlapping weapon, takes care of host */
	void SetOverlappingWeapon( AWeapon* InOverlappingWeapon );

	/** Called by PlayerController to equip weapon */
	void EquipWeapon() const;

	/** True if character has equipped weapon */
	bool IsWeaponEquipped() const;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** Widget displayed over character's head */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> OverheadWidget;

private:
	/** Camera boom */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** Third person follow camera */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Weapon which this character is currently overlapping */
	UPROPERTY( ReplicatedUsing = OnRep_OVerlappingWeapon )
	TObjectPtr<AWeapon> OverlappingWeapon;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UCombatComponent> Combat;

	/** OnRep notify for overlapping weapon */
	UFUNCTION()
	void OnRep_OverlappingWeapon( AWeapon* LastWeapon ) const;

	UFUNCTION( Server, Reliable )
	void ServerEquip() const;
};
