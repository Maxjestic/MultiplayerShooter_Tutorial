// Maxjestic's copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

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
	 * Sets up: Camera with spring arm
	 */
	ABlasterCharacter();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	//~ End AActor Interface

	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	//~ End APawn Interface

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

private:
	/** Camera boom */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** Third person follow camera */
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<UCameraComponent> CameraComponent;
};
