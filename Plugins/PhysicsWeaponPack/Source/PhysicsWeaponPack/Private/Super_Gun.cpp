// Fill out your copyright notice in the Description page of Project Settings.


#include "Super_Gun.h"
#include "PhysicsCharacter.h"
#include "Components/InputComponent.h"
#include "Engine/InputDelegateBinding.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  // Create a gun mesh component
  GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
  GunMesh->bCastDynamicShadow = false;
  GunMesh->CastShadow = false;
  RootComponent = GunMesh;

  Gun_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
  Gun_MuzzleLocation->SetupAttachment(GunMesh);
  Gun_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

void ASuper_Gun::OnFireKeyPressed_Implementation()
{
  UE_LOG(LogTemp, Log, TEXT("Attack Key Pressed"))
}

void ASuper_Gun::OnFireKeyReleased_Implementation()
{
  UE_LOG(LogTemp, Log, TEXT("Attack Key Released"))
}

void ASuper_Gun::SetupGunInput(class APhysicsCharacter* Player)
{
  if (Player)
  {
    if (!Player->GetCurrentGun())
    {
      Player->EquipGun(this);
    }

    if (!InputComponent)
    {
      InputComponent = NewObject<UInputComponent>(this);
      InputComponent->RegisterComponent();
      InputComponent->bBlockInput = bBlockInput;
      InputComponent->Priority = InputPriority;

      UBlueprintGeneratedClass* BGClass = Cast<UBlueprintGeneratedClass>(GetClass());

      if (BGClass != NULL)
      {
        UInputDelegateBinding::BindInputDelegates(BGClass, InputComponent);
      }
    }
    InputComponent->BindAction("Fire", IE_Pressed, this, &ASuper_Gun::OnFireKeyPressed);
    InputComponent->BindAction("Fire", IE_Released, this, &ASuper_Gun::OnFireKeyReleased);
  }
}

void ASuper_Gun::EnableGunInput()
{
  APlayerController* PC = GetWorld()->GetFirstPlayerController();

  if (PC)
  {
    this->EnableInput(PC);
  }
}

void ASuper_Gun::DisableGunInput()
{
  APlayerController* PC = GetWorld()->GetFirstPlayerController();

  if (PC)
  {
    this->DisableInput(PC);
  }
}

