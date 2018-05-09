#pragma once
#include "IObject.h"
class Fire : public IObject
{
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( float height ) override;
};