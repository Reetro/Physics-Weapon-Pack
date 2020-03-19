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

  /** Location on VR gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* VR_MuzzleLocation;

  /** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* VR_Gun;

  /** Location on gun mesh where projectiles should spawn. */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USceneComponent* Gun_MuzzleLocation;

  /** Gun mesh: 1st person view (seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class USkeletalMeshComponent* GunMesh;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Fire Functions")
  void OnGunFire();
  void OnGunFire_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
