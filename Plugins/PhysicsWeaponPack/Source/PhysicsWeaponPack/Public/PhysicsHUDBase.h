// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PhysicsHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICSWEAPONPACK_API APhysicsHUDBase : public AHUD
{
  GENERATED_BODY()

   APhysicsHUDBase();

  /** Primary draw call for the HUD */
  virtual void DrawHUD() override;

private:
  /** Crosshair asset pointer */
  class UTexture2D* CrosshairTex;
	
};
