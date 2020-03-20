// Fill out your copyright notice in the Description page of Project Settings.


#include "Super_WeaponPickup.h"
#include "Super_Gun.h"
#include "Components/SphereComponent.h"
#include "PhysicsCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuper_WeaponPickup::ASuper_WeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation Pivot Point"));
  RootComponent = SceneComp;

  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
  SphereComp->SetupAttachment(RootComponent);
  SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);

  WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
  WeaponMesh->SetupAttachment(RootComponent);
  WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  RoatationRate = FRotator(0.0f, 180.0f, 0.0f);
  RotationSpeed = 1.0f;
}

void ASuper_WeaponPickup::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  // Rotate Actor
  this->AddActorLocalRotation(RoatationRate * DeltaTime * RotationSpeed);
}

void ASuper_WeaponPickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor)
  {
    APhysicsCharacter* CurrentPlayer = Cast<APhysicsCharacter>(OtherActor);

    if (CurrentPlayer)
    {
      if (GunToGive)
      {
        ASuper_Gun* LocalGun = GetWorld()->SpawnActor<ASuper_Gun>(GunToGive, FVector(0), FRotator(0));

        if (LocalGun)
        {
          CurrentPlayer->EquipGun(LocalGun);
          Destroy();
        }
      }
      else
      {

      }
    }
  }
}

// Called when the game starts or when spawned
void ASuper_WeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
  SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASuper_WeaponPickup::OnOverlapBegin);
}
