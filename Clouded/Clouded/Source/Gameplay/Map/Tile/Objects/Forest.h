#pragma once
#include "IObject.h"
class Tree;
class Forest : public IObject
{
public:
  Forest( const char* model );
  virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) override;
  virtual void Draw( D3D11Renderer& gfx, float height ) override;

  std::vector<Tree*> trees;
};