// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Super_Gun.generated.h"

UCLASS()
class PHYSICSWEAPONPACK_API ASuper_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuper_Gun();

  /* Location on gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* Gun_MuzzleLocation;

  /* Gun mesh: 1st person view (seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* GunMesh;

  /* Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = "Gun Settings")
  TSubclassOf<class ASuper_Projectile> ProjectileClass;

  /* Gun muzzle's offset from the characters location */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Settings")
  FVector GunOffset;

  /* Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Settings")
  class USoundBase* FireSound;

  /* AnimMontage to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Settings")
  class UAnimMontage* FireAnimation;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Fire Events")
  void OnFireKeyPressed();
  virtual void OnFireKeyPressed_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Fire Events")
  void OnFireKeyReleased();
  virtual void OnFireKeyReleased_Implementation();

  /* Creates a input component on the gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void SetupGunInput(class APhysicsCharacter* Player);

  /* Gets the player controller and enables gun input */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void EnableGunInput();

  /* Gets the player controller and disables gun input */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void DisableGunInput();

  /* Will spawn the selected projectile class */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  class ASuper_Projectile* SpawnProjectile();

  /* Returns the current player holding the gun */
  UFUNCTION(BlueprintPure, Category = "Gun Functions")
 class APhysicsCharacter* GetCurrentPlayer();

private:

  class APhysicsCharacter* CurrentPlayer;

};
