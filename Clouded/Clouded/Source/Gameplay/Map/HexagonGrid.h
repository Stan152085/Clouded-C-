#pragma once
#include "Math\Bounds.h"
using GridBounds = Bounds2D<ZeroBound, unsigned>;
struct HexagonTile;

class HexagonGrid
{
  HexagonGrid(GridBounds bounds_, float hex_size);
private:
  HexagonTile* tiles_;
  GridBounds bounds_;
  float hex_size_;
};