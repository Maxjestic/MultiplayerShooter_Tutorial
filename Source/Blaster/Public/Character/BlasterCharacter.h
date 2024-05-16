// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

class AWeapon;
class UWidgetComponent;
class UInputConfigDataAsset;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
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
	 * Sets up: CharacterMovement, Camera with spring arm, overhead widget
	 */
	ABlasterCharacter();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	//~ End AActor Interface

	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;
	//~ End APawn Interface

	//~ Begin ACharacter Interface
	virtual void Jump() override;
	//~ End ACharacter Interface

	/** Setter for overlapping weapon, takes care of host */
	void SetOverlappingWeapon( AWeapon* InOverlappingWeapon );

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/**
	 * Enhanced Input Properties
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Enhanced Input" )
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input" )
	TObjectPtr<UInputConfigDataAsset> InputActions;

	/** Widget displayed over character's head */
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> OverheadWidget;

private:
	/**
	 * Enhanced Input Callbacks
	 */
	void Move( const FInputActionValue& InputActionValue );
	void Look( const FInputActionValue& InputActionValue );

	/** Camera boom */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** Third person follow camera */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Weapon which this character is currently overlapping */
	UPROPERTY( ReplicatedUsing = OnRep_OVerlappingWeapon )
	TObjectPtr<AWeapon> OverlappingWeapon;

	/** OnRep notify for overlapping weapon */
	UFUNCTION()
	void OnRep_OverlappingWeapon( AWeapon* LastWeapon ) const;
};
