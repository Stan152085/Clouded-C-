#include "stdafx.h"
#include "City.h"
#include "Building.h"

City::City( const char * model )
  :
  IObject( model )
{
}
bool City::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if ( buildings.size() < 3 )
  {
    bool chance = ( rand() % 50 ) > 20;
    if ( chance == true )
    {
      PlaceBuilding();
    }
  }
  for ( int i = static_cast< int >( buildings.size() ) - 1; i >= 0; i-- )
  {
    if ( buildings[i]->Update( grid, tile ) == false )
    {
      buildings.erase( buildings.begin() + i );
    }
  }
  if ( buildings.size() == 0 )
  {
    return false;
  }
  return true;
}

void City::Draw( D3D11Renderer& gfx, Vec3& position )
{
}

bool City::PlaceBuilding()
{
  // Implement random square shape in hexagon without overlapping other buildings check
  buildings.push_back( reinterpret_cast< Building* >( IObject::Create( IObject::kTree ) ) );
  return true;
}
