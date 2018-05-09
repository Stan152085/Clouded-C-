#pragma once
#include<vector>

class ITileState;
class IOccupation;
struct BufferedTileData;
class IObject;
class HexagonTile
{
public:
  HexagonTile();
  void WetnessUpdate(BufferedTileData& target_state);
  void Update();
  ITileState* state;
  std::vector<IObject*> objects;
};

struct BufferedTileData
{
  float delta_wetness;
};