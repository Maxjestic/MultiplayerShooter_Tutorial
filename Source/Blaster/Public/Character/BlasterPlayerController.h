// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class UInputConfigDataAsset;
class UInputMappingContext;
struct FInputActionValue;
/**
 * Player controller class for Blaster
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABlasterPlayerController();

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	//~ End AActor Interface

	/**
	 * Enhanced Input Properties
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Enhanced Input" )
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input" )
	TObjectPtr<UInputConfigDataAsset> InputActions;

private:
	/**
	 * Enhanced Input Callbacks
	 */
	void Move( const FInputActionValue& InputActionValue );
	void Look( const FInputActionValue& InputActionValue );
	void Jump();
	void Equip();
};
