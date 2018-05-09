#include "stdafx.h"
#include "HexagonTile.h"
#include "States/ITileState.h"
#include "States/StateConstructor.h"
#include "Objects/IObject.h"

HexagonTile::HexagonTile()
{
  float random = (rand() % 101) / 100.0f;
  state = StateConstructor::Create(random);
}

void HexagonTile::WetnessUpdate(BufferedTileData& target_state)
{
  target_state.delta_wetness = state->wetness();
}

void HexagonTile::Update( HexagonGrid* grid )
{
  ITileState* new_state = state->Update(grid);
  if (new_state != nullptr)
  {
    delete state;
    state = new_state;
  }
  for ( IObject* object : objects )
  {
    object->Update(grid, this);
  }
}
