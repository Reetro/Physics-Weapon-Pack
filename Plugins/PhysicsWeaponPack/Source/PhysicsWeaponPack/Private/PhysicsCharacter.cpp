// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCharacter.h"
#include "Super_Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Super_Gun.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APhysicsCharacter::APhysicsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  // Set size for collision capsule
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  // set our turn rates for input
  BaseTurnRate = 45.f;
  BaseLookUpRate = 45.f;

  // Create a CameraComponent	
  FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
  PlayerArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Arms"));
  PlayerArms->SetOnlyOwnerSee(true);
  PlayerArms->SetupAttachment(FirstPersonCameraComponent);
  PlayerArms->bCastDynamicShadow = false;
  PlayerArms->CastShadow = false;
  PlayerArms->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
  PlayerArms->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


  // Default offset from the character location for projectiles to spawn
  GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

// Called when the game starts or when spawned
void APhysicsCharacter::BeginPlay()
{
	Super::BeginPlay();

  SpawnStartingWeapon();
}

// Called to bind functionality to input
void APhysicsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

  // set up gameplay key bindings
  check(PlayerInputComponent);

  // Bind jump events
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  // Bind movement events
  PlayerInputComponent->BindAxis("MoveForward", this, &APhysicsCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &APhysicsCharacter::MoveRight);

  // We have 2 versions of the rotation bindings to handle different kinds of devices differently
  // "turn" handles devices that provide an absolute delta, such as a mouse.
  // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("TurnRate", this, &APhysicsCharacter::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("LookUpRate", this, &APhysicsCharacter::LookUpAtRate);
}

void APhysicsCharacter::MoveForward(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    AddMovementInput(GetActorForwardVector(), Value);
  }
}

void APhysicsCharacter::MoveRight(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    AddMovementInput(GetActorRightVector(), Value);
  }
}

void APhysicsCharacter::TurnAtRate(float Rate)
{
  // calculate delta for this frame from the rate information
  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APhysicsCharacter::LookUpAtRate(float Rate)
{
  // calculate delta for this frame from the rate information
  AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APhysicsCharacter::OnFire()
{
  if (CurrentGun)
  {
    CurrentGun->OnFireKeyPressed();
  }
}

ASuper_Gun* APhysicsCharacter::GetCurrentGun()
{
  return CurrentGun;
}

void APhysicsCharacter::EquipGun(ASuper_Gun* GunToEquip)
{
  if (CurrentGun)
  {
    UnEquipGun();

    PlayerArms->SetHiddenInGame(false);

    CurrentGun = GunToEquip;
    CurrentGun->SetupGunInput(this);
    CurrentGun->EnableGunInput();

    CurrentGun->AttachToComponent(PlayerArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
  }
  else
  {
    PlayerArms->SetHiddenInGame(false);

    CurrentGun = GunToEquip;
    CurrentGun->SetupGunInput(this);
    CurrentGun->EnableGunInput();

    CurrentGun->AttachToComponent(PlayerArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
  }
}

void APhysicsCharacter::UnEquipGun()
{
  if (CurrentGun)
  {
    CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentGun->Destroy();
  }
}

void APhysicsCharacter::SpawnStartingWeapon()
{
  if (StartingGun)
  {
    ASuper_Gun* LocalGun = GetWorld()->SpawnActor<ASuper_Gun>(StartingGun, FVector(0), FRotator(0));

    EquipGun(LocalGun);
  }
}

