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

  /** Location on gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* Gun_MuzzleLocation;

  /** Gun mesh: 1st person view (seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* GunMesh;

  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = "Projectile")
  TSubclassOf<class ASuper_Projectile> ProjectileClass;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Fire Events")
  void OnFireKeyPressed();
  void OnFireKeyPressed_Implementation();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Fire Events")
  void OnFireKeyReleased();
  void OnFireKeyReleased_Implementation();

  /* Creates a input component on the gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void SetupGunInput(class APhysicsCharacter* Player);

  /* Gets the player controller and enables gun input */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void EnableGunInput();

  /* Gets the player controller and disables gun input */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void DisableGunInput();
};
