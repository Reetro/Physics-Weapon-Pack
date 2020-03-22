// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABlackHoleBase::ABlackHoleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
  InnerSphereComponent->SetSphereRadius(100);
  RootComponent = InnerSphereComponent;

  OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
  OuterSphereComponent->SetSphereRadius(3000);
  OuterSphereComponent->SetupAttachment(RootComponent);

  ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));

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
      // the component we are looking for! It needs to be simulating in order to apply forces.

      const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
 
      PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, BlackHoleStrength, ERadialImpulseFalloff::RIF_Constant, true);
    }
  }
}

void ABlackHoleBase::OnInnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor)
  {
    OtherActor->Destroy();
  }
}
