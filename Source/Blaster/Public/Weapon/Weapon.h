// Copyright Maxjestic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UWidgetComponent;
class USphereComponent;

/** Enum defining current state of the weapon */
UENUM( BlueprintType )
enum class EWeaponState : uint8
{
	EWS_Initial UMETA( DisplayName = "Initial State" ),
	EWS_Equipped UMETA( DisplayName = "Equipped" ),
	EWS_Dropped UMETA( DisplayName = "Dropped" ),

	EWS_MAX UMETA( DisplayName = "DefaultMAX" )
};

/**
 * Base weapon class
 */
UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 * Sets: Replication, weapon mesh, area sphere
	 */
	AWeapon();

	//~ Begin AActor Interface
	virtual void Tick( float DeltaTime ) override;
	//~ End AActor Interface

	/** Shows/hides pick up widget */
	void ShowPickupWidget(const bool bShowWidget) const;

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	/** Callback to sphere overlap */
	UFUNCTION()
	virtual void OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	UFUNCTION()
	virtual void OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex );
	
private:
	/** Represents weapon in world */
	UPROPERTY( VisibleAnywhere, Category = "Weapon Properties" )
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/** When overlapped - allows player to pick up the weapon */
	UPROPERTY( VisibleAnywhere, Category = "Weapon Properties" )
	TObjectPtr<USphereComponent> AreaSphere;

	/** Current state of the weapon */
	UPROPERTY( VisibleAnywhere, Category = "Weapon Properties" )
	EWeaponState WeaponState = EWeaponState::EWS_Initial;

	UPROPERTY( VisibleAnywhere, Category = "Weapon Properties" )
	TObjectPtr<UWidgetComponent> PickupWidget;
};
