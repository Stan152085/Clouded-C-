#include "stdafx.h"
#include "Fire.h"

void Fire::Update()
{
}

void Fire::Draw( float height )
{
  Transform draw_transform = transform.Translated( 0, height, 0 );
  // Draw particle
}
