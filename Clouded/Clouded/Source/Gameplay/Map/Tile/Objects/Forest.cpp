#include "stdafx.h"
#include "Forest.h"

Forest::Forest( const char * model )
  :
  IObject( model )
{
}

bool Forest::Update( HexagonGrid* grid, HexagonTile* tile )
{
  return false;
}

void Forest::Draw( D3D11Renderer& gfx, float height )
{
}
