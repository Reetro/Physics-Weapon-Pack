// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsGunController.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICSWEAPONPACK_API APhysicsGunController : public APlayerController
{
  GENERATED_BODY()

    virtual void BuildInputStack(TArray<UInputComponent*>& InputStack) override;
};
