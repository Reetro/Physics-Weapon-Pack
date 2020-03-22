// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Super_Projectile.generated.h"

UCLASS()
class PHYSICSWEAPONPACK_API ASuper_Projectile : public AActor
{
	GENERATED_BODY()

  /** Sphere collision component */
  UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
  class USphereComponent* CollisionComp;

  /** Projectile movement component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
  class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	ASuper_Projectile();

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Settings")
  float Damage;

  /** called when projectile hits something */
  UFUNCTION()
  void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

  /* Called when this actor hit's an object */
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile Events")
  void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
  virtual void OnActorHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

  /** Returns CollisionComp subobject **/
  FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
  /** Returns ProjectileMovement subobject **/
  FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
