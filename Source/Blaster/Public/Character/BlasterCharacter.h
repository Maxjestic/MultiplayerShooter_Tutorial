// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

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
	 * Sets up: CharacterMovement, Camera with spring arm
	 */
	ABlasterCharacter();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	//~ End AActor Interface

	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;
	//~ End APawn Interface

	//~ Begin ACharacter Interface
	virtual void Jump() override;
	//~ End ACharacter Interface

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/**
	 * Enhanced Input Properties
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Enhanced Input" )
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputConfigDataAsset> InputActions;

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
};
