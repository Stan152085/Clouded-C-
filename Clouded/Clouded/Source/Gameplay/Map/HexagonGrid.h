#pragma once
#include "Math\Bounds.h"
namespace resources
{
  class AssetManager;
}
using GridBounds = Bounds2D<ZeroBound, size_t>;
class HexagonTile;
struct BufferedTileData;
class D3D11Renderer;

class HexagonGrid
{
public:
  HexagonGrid(GridBounds bounds_, float hex_size,resources::AssetManager& asset_manager);
  void Draw( D3D11Renderer& gfx );
  void DebugDraw(D3D11Renderer& gfx);
  void Update();
  void WetnessUpdate();
private:
  HexagonTile* tiles_;
  BufferedTileData* buffered_data_;
  GridBounds bounds_;
  float hex_size_;
};