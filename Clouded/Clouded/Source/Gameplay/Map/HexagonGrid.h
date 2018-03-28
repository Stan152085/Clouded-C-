#pragma once
#include "Math\Bounds.h"
using GridBounds = Bounds2D<ZeroBound, size_t>;
class HexagonTile;
struct BufferedTileData;
class D3D11Renderer;

class HexagonGrid
{
public:
  HexagonGrid(GridBounds bounds_, float hex_size);
  void DebugDraw(D3D11Renderer& gfx);
  void Update();
  void WetnessUpdate();
private:
  HexagonTile* tiles_;
  BufferedTileData* buffered_data_;
  GridBounds bounds_;
  float hex_size_;
};