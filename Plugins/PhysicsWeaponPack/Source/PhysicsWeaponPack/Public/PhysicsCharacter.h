// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsCharacter.generated.h"

UCLASS()
class PHYSICSWEAPONPACK_API APhysicsCharacter : public ACharacter
{
	GENERATED_BODY()

  /** Pawn mesh: 1st person view (arms; seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* Mesh1P;

  /** Gun mesh: 1st person view (seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* FP_Gun;

  /** Location on gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* FP_MuzzleLocation;

  /** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* VR_Gun;

  /** Location on VR gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* VR_MuzzleLocation;

  /** First person camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  class UCameraComponent* FirstPersonCameraComponent;

  /** Motion controller (right hand) */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UMotionControllerComponent* R_MotionController;

  /** Motion controller (left hand) */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UMotionControllerComponent* L_MotionController;

public:
	// Sets default values for this character's properties
	APhysicsCharacter();

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

  /** Resets HMD orientation and position in VR. */
  void OnResetVR();

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
  FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
  /** Returns FirstPersonCameraComponent subobject **/
  FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};