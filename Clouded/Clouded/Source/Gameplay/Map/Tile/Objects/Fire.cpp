#include "stdafx.h"
#include "Fire.h"

Fire::Fire( const char * model )
  :
  IObject( model )
{
}

bool Fire::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return false;
}

void Fire::Draw( D3D11Renderer& gfx, float height )
{
  Transform draw_transform = transform.Translated( 0, height, 0 );
  // Draw particle
}
