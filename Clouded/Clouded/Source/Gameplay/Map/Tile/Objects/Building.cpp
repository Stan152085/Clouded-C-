#include "stdafx.h"
#include "Building.h"

Building::Building( const char * model )
  :
  IObject( model )
{
}

bool Building::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return false;
}

void Building::Draw( D3D11Renderer& gfx, float height )
{
}
