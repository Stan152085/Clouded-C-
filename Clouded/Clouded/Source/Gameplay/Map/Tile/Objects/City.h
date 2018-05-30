#pragma once
#include "IObject.h"
class Building;
class City : public IObject
{
public:
  City( const char* model );
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( D3D11Renderer& gfx, float height ) override;
  std::vector<Building*> buildings;
};