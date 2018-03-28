#include "stdafx.h"
#include "HexagonTile.h"
#include "States\ITileState.h"
#include "States\StateConstructor.h"

HexagonTile::HexagonTile()
{
  float random = (rand() % 101) / 100.0f;
  state = StateConstructor::Create(random);
}

void HexagonTile::WetnessUpdate(BufferedTileData& target_state)
{
  target_state.delta_wetness = state->wetness();
  // Accumulate from surrounding tiles...

  //
}

void HexagonTile::Update()
{
  ITileState* new_state = state->Update();
  if (new_state != nullptr)
  {
    delete state;
    state = new_state;
  }
}
