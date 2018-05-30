#pragma once
#include "IObject.h"
class Tree : public IObject
{
public:
  Tree( const char* model );
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( D3D11Renderer& gfx, float height ) override;
  void Dying();
  void Growing(float wetness);
  float life;
};