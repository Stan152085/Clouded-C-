#pragma once
#include "ITileState.h"
class GrassState : public ITileState
{
public:
  GrassState(float wetness);
  ITileState* Update( HexagonGrid* grid, HexagonTile* tile ) override;
  void Draw( D3D11Renderer& gfx, const Vec2& position ) override;
  virtual float CalculateWaterTransition(ITileState* other) override;

  static float up_threshold_;
  static float down_threshold_;
};