// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsHUDBase.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

APhysicsHUDBase::APhysicsHUDBase()
{
  static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/PhysicsWeaponPack/Textures/FirstPersonCrosshair"));
  CrosshairTex = CrosshairTexObj.Object;
  check(CrosshairTex);
}

void APhysicsHUDBase::DrawHUD()
{
  Super::DrawHUD();

  // Draw very simple crosshair

  // find center of the Canvas
  const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

  // offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
  const FVector2D CrosshairDrawPosition((Center.X),
    (Center.Y + 20.0f));

  // draw the crosshair
  FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
  TileItem.BlendMode = SE_BLEND_Translucent;
  Canvas->DrawItem(TileItem);
}
