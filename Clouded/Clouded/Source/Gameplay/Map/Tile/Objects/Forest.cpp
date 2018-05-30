#include "stdafx.h"
#include "Forest.h"
#include "Tree.h"

Forest::Forest( const char * model )
  :
  IObject( model )
{
}

bool Forest::Update( HexagonGrid* grid, HexagonTile* tile )
{
  if ( trees.size() < 3 )
  {
    bool chance = ( rand() % 50 ) > 20;
    if ( chance == true )
    {
      PlaceTree();
    }
  }
  for ( int i = static_cast< int >( trees.size() ) - 1; i >= 0; i-- )
  {
    if ( trees[i]->Update( grid, tile ) == false )
    {
      trees.erase( trees.begin() + i );
    }
  }
  if ( trees.size() == 0 )
  {
    return false;
  }
  return true;
}

void Forest::Draw( D3D11Renderer& gfx, Vec3& position )
{
  for ( Tree* trees : trees )
  {
    trees->Draw( gfx, Vec3( position.x, position.y, position.z ) );
  }
}

bool Forest::PlaceTree()
{
  // Implement random circle shape in hexagon without overlapping other tree circles check
  trees.push_back( reinterpret_cast< Tree* >( IObject::Create( IObject::kTree ) ) );
  return true;
}
