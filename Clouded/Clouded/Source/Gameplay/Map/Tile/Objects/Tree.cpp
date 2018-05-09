#include "stdafx.h"
#include "Tree.h"
#include "Gameplay/Map/Tile/HexagonTile.h"
#include "Gameplay/Map/Tile/States/ITileState.h"
#include "Core/Time.h"

bool Tree::Update( HexagonGrid* grid, HexagonTile* tile )
{
  switch ( tile->state->Type() )
  {
  case ITileState::Water:
  case ITileState::Desert:
    Dying();
    break;
  case ITileState::Savannah:
  case ITileState::Grass:
  case ITileState::Swamp:
    Growing(tile->state->wetness());
    break;
    break;
  }
  if ( life <= 0.0f )
  {
    return false;
  }
  return true;
}

void Tree::Draw( float height )
{
  Transform draw_transform = transform.Translated( 0, height, 0 );
  // Draw model
}

void Tree::Dying()
{
  life -= 1.0f * Time::dt;
}

void Tree::Growing( float dt )
{
  if ( life < 100.f )
  {
    life += 1.0f* Time::dt;
  }
}
