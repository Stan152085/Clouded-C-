#pragma once
#include "Core/Transform.h"
class HexagonGrid;
class HexagonTile;
class IObject
{
public:
  virtual void Update( HexagonGrid* grid, HexagonTile* tile) = 0;
  virtual void Draw(float height) = 0;

  Transform transform;
};