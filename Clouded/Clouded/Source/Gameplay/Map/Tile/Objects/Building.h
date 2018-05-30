#pragma once
#include "IObject.h"
class Building : public IObject
{
public:
  Building( const char* model );
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( D3D11Renderer& gfx, Vec3& position ) override;
};