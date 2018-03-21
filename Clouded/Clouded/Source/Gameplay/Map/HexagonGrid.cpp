#include "stdafx.h"
#include "HexagonGrid.h"
#include "HexagonTile.h"

#include "Graphics\renderer.h"
#include "Math\math_defines.h"

HexagonGrid::HexagonGrid(GridBounds bounds_, float hex_size)
  :
  tiles_(reinterpret_cast<HexagonTile*>(malloc(bounds_.GetMaxX()*bounds_.GetMaxY() * sizeof(HexagonTile)))),
  bounds_(bounds_),
  hex_size_(hex_size)
{
  size_t width = bounds_.GetMaxX();
  size_t height = bounds_.GetMaxY();
  int index = 0;
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

void HexagonGrid::DebugDraw(D3D11Renderer& gfx)
{
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
      Vec2 pos = Vec2((x + row_offset / 2.0f)*horizontal_distance, y*vertical_distance);
      Vec2 points[6];
      for (int i = 0; i < 6; ++i)
      {
        float angle_deg = (60.0f * i) + 30.0f;
        float angle_rad = glm::pi<float>() / 180.0f * angle_deg;
        points[i] = (Vec2(pos.x + hex_size_ * cos(angle_rad), pos.y + hex_size_ * sin(angle_rad)));
      }

      gfx.AddLine(Vec3(points[0],0), Vec3(points[1],0));
      gfx.AddLine(Vec3(points[1],0), Vec3(points[2],0));
      gfx.AddLine(Vec3(points[2],0), Vec3(points[3],0));
      gfx.AddLine(Vec3(points[3],0), Vec3(points[4],0));
      gfx.AddLine(Vec3(points[4],0), Vec3(points[5],0));
      gfx.AddLine(Vec3(points[5],0), Vec3(points[0],0));
    }
  }
}
