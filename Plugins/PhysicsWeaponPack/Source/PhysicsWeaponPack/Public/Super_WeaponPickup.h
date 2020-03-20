// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Super_WeaponPickup.generated.h"

UCLASS()
class PHYSICSWEAPONPACK_API ASuper_WeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuper_WeaponPickup();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  class USkeletalMeshComponent* WeaponMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  class USceneComponent* SceneComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  class USphereComponent* SphereComp;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Setting")
  float RotationSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Setting")
  FRotator RoatationRate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Setting")
  TSubclassOf<class ASuper_Gun> GunToGive;
 
  virtual void Tick(float DeltaTime) override;

  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
