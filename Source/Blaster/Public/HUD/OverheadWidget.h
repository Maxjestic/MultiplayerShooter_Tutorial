// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

class UTextBlock;
/**
 * Widget displayed over player heads
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Text block used to display some information */
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UTextBlock> DisplayText;

	/** Displays given text in DisplayText text block */
	void SetDisplayText( const FString& TextToDisplay ) const;

	/** Check InPawn net role and passes it to SetDisplayText */
	UFUNCTION( BlueprintCallable )
	void ShowPlayerAndNetRole( const APawn* InPawn ) const;
};
