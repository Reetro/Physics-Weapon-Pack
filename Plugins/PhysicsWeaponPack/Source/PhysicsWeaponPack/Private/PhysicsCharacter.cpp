// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCharacter.h"
#include "Super_Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId


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
  Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
  Mesh1P->SetOnlyOwnerSee(true);
  Mesh1P->SetupAttachment(FirstPersonCameraComponent);
  Mesh1P->bCastDynamicShadow = false;
  Mesh1P->CastShadow = false;
  Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
  Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

  // Create a gun mesh component
  FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
  FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
  FP_Gun->bCastDynamicShadow = false;
  FP_Gun->CastShadow = false;
  // FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
  FP_Gun->SetupAttachment(RootComponent);

  FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
  FP_MuzzleLocation->SetupAttachment(FP_Gun);
  FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

  // Default offset from the character location for projectiles to spawn
  GunOffset = FVector(100.0f, 0.0f, 10.0f);

  // Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
  // are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

  // Create VR Controllers.
  R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
  R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
  R_MotionController->SetupAttachment(RootComponent);
  L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
  L_MotionController->SetupAttachment(RootComponent);

  // Create a gun and attach it to the right-hand VR controller.
  // Create a gun mesh component
  VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
  VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
  VR_Gun->bCastDynamicShadow = false;
  VR_Gun->CastShadow = false;
  VR_Gun->SetupAttachment(R_MotionController);
  VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

  VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
  VR_MuzzleLocation->SetupAttachment(VR_Gun);
  VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
  VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

  // Uncomment the following line to turn motion controllers on by default:
  //bUsingMotionControllers = true;

}

// Called when the game starts or when spawned
void APhysicsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
  //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
  FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

  // Show or hide the two versions of the gun based on whether or not we're using motion controllers.
  if (bUsingMotionControllers)
  {
    VR_Gun->SetHiddenInGame(false, true);
    Mesh1P->SetHiddenInGame(true, true);
  }
  else
  {
    VR_Gun->SetHiddenInGame(true, true);
    Mesh1P->SetHiddenInGame(false, true);
  }
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

  // Bind fire event
  PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APhysicsCharacter::OnFire);

  PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APhysicsCharacter::OnResetVR);

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
  // try and fire a projectile
  if (ProjectileClass != NULL)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      if (bUsingMotionControllers)
      {
        const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
        const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
        World->SpawnActor<ASuper_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
      }
      else
      {
        const FRotator SpawnRotation = GetControlRotation();
        // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
        const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

        //Set Spawn Collision Handling Override
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // spawn the projectile at the muzzle
        World->SpawnActor<ASuper_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
      }
    }
  }
}

void APhysicsCharacter::OnResetVR()
{
  UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}