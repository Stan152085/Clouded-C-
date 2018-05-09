#include "stdafx.h"
#include "Tree.h"

void Tree::Update( HexagonGrid* grid, HexagonTile* tile )
{
}

void Tree::Draw( float height )
{
  Transform draw_transform = transform.Translated( 0, height, 0 );
  // Draw model
}
