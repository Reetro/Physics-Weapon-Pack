// Fill out your copyright notice in the Description page of Project Settings.


#include "Super_WeaponPickup.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASuper_WeaponPickup::ASuper_WeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation Pivot Point"));
  RootComponent = SceneComp;

  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
  SphereComp->SetupAttachment(RootComponent);

  WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
  WeaponMesh->SetupAttachment(RootComponent);

  RoatationRate = FRotator(0.0f, 180.0f, 0.0f);
  RotationSpeed = 1.0f;
}

void ASuper_WeaponPickup::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  // Rotate Actor
  this->AddActorLocalRotation(RoatationRate * DeltaTime * RotationSpeed);
}

// Called when the game starts or when spawned
void ASuper_WeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}
