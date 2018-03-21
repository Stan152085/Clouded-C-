#pragma once
#include "Math\Bounds.h"
using GridBounds = Bounds2D<ZeroBound, unsigned>;
struct HexagonTile;
class D3D11Renderer;

class HexagonGrid
{
public:
  HexagonGrid(GridBounds bounds_, float hex_size);
  void  DebugDraw(D3D11Renderer& gfx);
private:
  HexagonTile* tiles_;
  GridBounds bounds_;
  float hex_size_;
};