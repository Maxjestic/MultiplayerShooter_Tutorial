// Maxjestic's copyright.


#include "Character/BlasterAnimInstance.h"

#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation( float DeltaSeconds )
{
	Super::NativeUpdateAnimation( DeltaSeconds );

	if(!BlasterCharacter)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
		return;
	}

	Speed = UKismetMathLibrary::VSizeXY(BlasterCharacter->GetVelocity());
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
	bIsMoving = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
}
