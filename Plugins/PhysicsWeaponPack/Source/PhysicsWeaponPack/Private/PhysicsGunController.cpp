// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsGunController.h"
#include "Super_Gun.h"
#include "PhysicsCharacter.h"

void APhysicsGunController::BuildInputStack(TArray<UInputComponent*>& InputStack)
{
  APhysicsCharacter* ControlledCharacter = Cast<APhysicsCharacter>(GetPawnOrSpectator());

  if (ControlledCharacter)
  {
    ASuper_Gun* ControlledGun = ControlledCharacter->GetCurrentGun();

    if (ControlledGun)
    {
      if (ControlledGun->InputComponent)
      {
        InputStack.Push(ControlledGun->InputComponent);
      }
    }
  }
  Super::BuildInputStack(InputStack);
}
