// Fill out your copyright notice in the Description page of Project Settings.


#include "Super_Gun.h"
#include "PhysicsCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Super_Projectile.h"
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

  // Default offset from the character location for projectiles to spawn
  GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void ASuper_Gun::OnFireKeyPressed_Implementation()
{
  SpawnProjectile();
}

void ASuper_Gun::OnFireKeyReleased_Implementation()
{
  // For Use in children is called when fire key is released
}

void ASuper_Gun::SetupGunInput(class APhysicsCharacter* Player)
{
  if (Player)
  {
    if (!Player->GetCurrentGun())
    {
      Player->EquipGun(this);
    }

    CurrentPlayer = Player;

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

class ASuper_Projectile* ASuper_Gun::SpawnProjectile()
{
  if (ProjectileClass)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      if (CurrentPlayer)
      {
        const FRotator SpawnRotation = CurrentPlayer->GetControlRotation();
        // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
        const FVector SpawnLocation = ((Gun_MuzzleLocation != nullptr) ? Gun_MuzzleLocation->GetComponentLocation() : CurrentPlayer->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

        //Set Spawn Collision Handling Override
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // spawn the projectile at the muzzle
        ASuper_Projectile* SpawnedProject = World->SpawnActor<ASuper_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

        // try and play the sound if specified
        if (FireSound != NULL)
        {
          UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }

        // try and play a firing animation if specified
        if (FireAnimation != NULL)
        {
          // Get the animation object for the arms mesh
          UAnimInstance* AnimInstance = CurrentPlayer->GetPlayerArms()->GetAnimInstance();
          if (AnimInstance != NULL)
          {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
          }
        }

        return SpawnedProject;
      }
      else
      {
        return nullptr;
      }
    }
    else
    {
      return nullptr;
    }
  }
  else
  {
    return nullptr;
  }
}

class APhysicsCharacter* ASuper_Gun::GetCurrentPlayer()
{
  return CurrentPlayer;
}

