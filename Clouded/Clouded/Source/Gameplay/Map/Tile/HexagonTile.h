#pragma once
class ITileState;
class IOccupation;
struct BufferedTileData;
class HexagonTile
{
public:
  HexagonTile();
  void WetnessUpdate(BufferedTileData& target_state);
  void Update();
  ITileState* state;
};

struct BufferedTileData
{
  float delta_wetness;
};