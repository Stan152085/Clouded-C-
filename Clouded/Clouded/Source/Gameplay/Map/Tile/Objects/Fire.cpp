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

void Fire::Draw( D3D11Renderer& gfx, Vec3& position )
{
  Transform draw_transform = transform.Translated( position );
  // Draw particle
}
