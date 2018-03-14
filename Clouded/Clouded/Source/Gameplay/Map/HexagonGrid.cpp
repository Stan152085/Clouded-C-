#include "stdafx.h"
#include "HexagonGrid.h"
#include "HexagonTile.h"

HexagonGrid::HexagonGrid(GridBounds bounds_, float hex_size)
  :
  tiles_(reinterpret_cast<HexagonTile*>(malloc(bounds_.GetMaxX()*bounds_.GetMaxY() * sizeof(HexagonTile)))),
  bounds_(bounds_),
  hex_size_(hex_size)
{
  size_t width = bounds_.GetMaxX();
  size_t height = bounds_.GetMaxY();
  int index = 0;
  float horizontal_distance;
  float vertical_distance;
  float hex_height = hex_size_ * 2.0f;
  float hex_width = hex_height * sqrtf(3.0f) / 2.0f;
  horizontal_distance = hex_width;
  vertical_distance = hex_height * .75f;
  for (size_t y = 0; y < bounds_.GetMaxY(); ++y)
  {
    int row_offset = y & 1;
    for (size_t x = 0; x < bounds_.GetMaxX(); ++x)
    {
      tiles_[index] = HexagonTile();
      ++index;
    }
  }
}
