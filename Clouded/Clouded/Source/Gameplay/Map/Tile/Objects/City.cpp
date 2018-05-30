#include "stdafx.h"
#include "City.h"

City::City( const char * model )
  :
  IObject( model )
{
}
bool City::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return false;
}

void City::Draw( D3D11Renderer& gfx, float height )
{
}
