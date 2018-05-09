#pragma once
#include "IObject.h"
class Building;
class City : public IObject
{
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( float height ) override;

  std::vector<Building*> buildings;
};