#pragma once
#include "IObject.h"
class Tree : public IObject
{
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( float height ) override;
  void Dying();
  void Growing(float wetness);
  float life;
};