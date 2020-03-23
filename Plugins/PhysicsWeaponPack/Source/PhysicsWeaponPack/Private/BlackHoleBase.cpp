// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleBase.h"
#include "Components/SphereComponent.h"
#include "PhysicsCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABlackHoleBase::ABlackHoleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   BlackHoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
   RootComponent = BlackHoleMesh;

  InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
  InnerSphereComponent->SetSphereRadius(100);
  InnerSphereComponent->SetupAttachment(RootComponent);

  OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
  OuterSphereComponent->SetSphereRadius(3000);
  OuterSphereComponent->SetupAttachment(RootComponent);

  // Set Static Mesh
  static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/PhysicsWeaponPack/Meshes/BlackHoleMesh"));
  check(Mesh.Succeeded());

  BlackHoleMesh->SetStaticMesh(Mesh.Object);
  BlackHoleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  BlackHoleStrength = -2000;
}

// Called when the game starts or when spawned
void ABlackHoleBase::BeginPlay()
{
	Super::BeginPlay();
	
  InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleBase::OnInnerSphereOverlap);
}

// Called every frame
void ABlackHoleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  // Find all overlapping components that can collide and may be physically simulating.
  TArray<UPrimitiveComponent*> OverlappingComps;
  OuterSphereComponent->GetOverlappingComponents(OverlappingComps);

  for (int32 i = 0; i < OverlappingComps.Num(); i++)
  {
    UPrimitiveComponent* PrimComp = OverlappingComps[i];
    if (PrimComp && PrimComp->IsSimulatingPhysics())
    {
      const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
 
      PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, BlackHoleStrength, ERadialImpulseFalloff::RIF_Constant, true);
    }
  }
}

void ABlackHoleBase::OnInnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor)
  {
    APhysicsCharacter* Player = Cast<APhysicsCharacter>(OtherActor);

    if (!Player)
    {
      OtherActor->Destroy();
    }
  }
}
