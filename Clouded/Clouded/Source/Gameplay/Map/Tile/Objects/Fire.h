#pragma once
#include "IObject.h"
class Fire : public IObject
{
public:
  Fire( const char* model );
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( D3D11Renderer& gfx, Vec3& position ) override;
};