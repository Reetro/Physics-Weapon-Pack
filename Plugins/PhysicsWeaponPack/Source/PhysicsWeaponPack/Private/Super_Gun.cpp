// Fill out your copyright notice in the Description page of Project Settings.


#include "Super_Gun.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  // Create a gun mesh component
  GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
  GunMesh->SetOnlyOwnerSee(true);
  GunMesh->bCastDynamicShadow = false;
  GunMesh->CastShadow = false;
  GunMesh->SetupAttachment(RootComponent);

  Gun_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
  Gun_MuzzleLocation->SetupAttachment(GunMesh);
  Gun_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

void ASuper_Gun::OnGunFire_Implementation()
{

}

// Called when the game starts or when spawned
void ASuper_Gun::BeginPlay()
{
	Super::BeginPlay();
	

}
