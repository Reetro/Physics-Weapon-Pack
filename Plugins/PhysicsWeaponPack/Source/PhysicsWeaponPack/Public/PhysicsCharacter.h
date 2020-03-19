// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsCharacter.generated.h"

class ASuper_Gun;

UCLASS()
class PHYSICSWEAPONPACK_API APhysicsCharacter : public ACharacter
{
	GENERATED_BODY()

  /** Pawn mesh: 1st person view (arms; seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* PlayerArms;

  /** First person camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  class UCameraComponent* FirstPersonCameraComponent;

  /** Motion controller (right hand) */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UMotionControllerComponent* R_MotionController;

  /** Motion controller (left hand) */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UMotionControllerComponent* L_MotionController;

  /* Returns the current gun the player has equipped */
  UFUNCTION(BlueprintPure, Category = "Weapon Vars")
  ASuper_Gun* GetCurrentGun();

  UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
  void EquipGun(ASuper_Gun* GunToEquip);

  UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
  void UnEquipGun();

public:
	// Sets default values for this character's properties
	APhysicsCharacter();

private:

  ASuper_Gun* CurrentGun;

public:
  /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
  float BaseTurnRate;

  /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
  float BaseLookUpRate;

  /** Gun muzzle's offset from the characters location */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  FVector GunOffset;

  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = "Projectile")
  TSubclassOf<class ASuper_Projectile> ProjectileClass;
  
  /* Gun player will start with */
  UPROPERTY(EditDefaultsOnly, Category = "Projectile")
  TSubclassOf<ASuper_Gun> StartingGun;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  class USoundBase* FireSound;

  /** AnimMontage to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  class UAnimMontage* FireAnimation;

  /** Whether to use motion controller location for aiming. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  uint32 bUsingMotionControllers : 1;

protected:

  /** Fires a projectile. */
  void OnFire();

  /** Handles moving forward/backward */
  void MoveForward(float Val);

  /** Handles stafing movement, left and right */
  void MoveRight(float Val);

  /**
   * Called via input to turn at a given rate.
   * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
   */
  void TurnAtRate(float Rate);

  /**
   * Called via input to turn look up/down at a given rate.
   * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
   */
  void LookUpAtRate(float Rate);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
  /** Returns Mesh1P subobject **/
  FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return PlayerArms; }
  /** Returns FirstPersonCameraComponent subobject **/
  FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
