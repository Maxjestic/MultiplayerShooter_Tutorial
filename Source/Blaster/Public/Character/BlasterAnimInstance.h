// Maxjestic's copyright.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

class ABlasterCharacter;
/**
 * Anim Instance for Blaster character
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//~ Begin UAnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ End UAnimInstance Interface

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ABlasterCharacter> BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAiming;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
};
