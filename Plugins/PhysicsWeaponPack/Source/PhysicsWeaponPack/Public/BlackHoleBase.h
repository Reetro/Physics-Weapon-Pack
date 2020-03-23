// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHoleBase.generated.h"

UCLASS()
class PHYSICSWEAPONPACK_API ABlackHoleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHoleBase();

  /* Negative value to make it pull towards the origin instead of pushing away */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Black Hole Settings", meta = (ClampMax = "-1"))
  float BlackHoleStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

  UPROPERTY(VisibleAnywhere, Category = "Components")
  class UStaticMeshComponent* BlackHoleMesh;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  class USphereComponent* InnerSphereComponent;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  class USphereComponent* SphereComponent;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  class USphereComponent* OuterSphereComponent;

  UFUNCTION()
  void OnInnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
