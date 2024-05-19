// Maxjestic's copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputConfigDataAsset.generated.h"

class UInputAction;

/**
 * Contains Input Action for Enhanced Input
 */
UCLASS()
class BLASTER_API UInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> JumpAction;	

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> EquipAction;	

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> CrouchAction;	

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> AimAction;	
};
