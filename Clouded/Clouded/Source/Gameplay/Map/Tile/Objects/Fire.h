#pragma once
#include "IObject.h"
class Fire : public IObject
{
  virtual void Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( float height ) override;
};