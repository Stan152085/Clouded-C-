#include "stdafx.h"
#include "Fire.h"

bool Fire::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return false;
}

void Fire::Draw( float height )
{
  Transform draw_transform = transform.Translated( 0, height, 0 );
  // Draw particle
}
