#pragma once
#include "IObject.h"
class Tree;
class Forest : public IObject
{
public:
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( float height ) override;

  std::vector<Tree*> trees;
};