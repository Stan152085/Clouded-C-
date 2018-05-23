#include "stdafx.h"
#include "HexagonTile.h"
#include "States/ITileState.h"
#include "Objects/IObject.h"

HexagonTile::HexagonTile()
{
  static float random = 0.f;
  state = ITileState::Create(random);
  random *= 0.8f;
}

void HexagonTile::WetnessUpdate(BufferedTileData& target_state)
{
  target_state.delta_wetness = state->wetness();
}

void HexagonTile::Update( HexagonGrid* grid )
{
  ITileState* new_state = state->Update( grid, this );
  if ( new_state != nullptr )
  {
    delete state;
    state = new_state;
  }
  for ( int i = static_cast< int >( objects.size() ) - 1; i >= 0; i-- )
  {
    if ( objects[i]->Update( grid, this ) == false )
    {
      objects.erase( objects.begin() + i );
    }
  }
}

void HexagonTile::Draw( D3D11Renderer& gfx, const Vec2& position )
{
  state->Draw( gfx, position );
}
