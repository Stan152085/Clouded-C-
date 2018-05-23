#include "stdafx.h"
#include "HexagonGrid.h"
#include "Tile/HexagonTile.h"
#include "Tile/States/ITileState.h"

#include "Resources/AssetManager.h"
#include "Graphics/renderer.h"
#include "Math/math_defines.h"
#include "Core/Time.h"

HexagonGrid::HexagonGrid( GridBounds bounds_, float hex_size, resources::AssetManager& asset_manager )
  :
  buffered_data_(reinterpret_cast<BufferedTileData*>(malloc(bounds_.GetMaxX()*bounds_.GetMaxY() * sizeof(BufferedTileData)))),
  bounds_(bounds_),
  hex_size_(hex_size)
{
  ITileState::Initialize(asset_manager);
  tiles_ = new HexagonTile[bounds_.GetMaxX()*bounds_.GetMaxY()];
  size_t width = bounds_.GetMaxX();
  size_t height = bounds_.GetMaxY();
  int index = 0;
  for (size_t y = 0; y < bounds_.GetMaxY(); ++y)
  {
    int row_offset = y & 1;
    for (size_t x = 0; x < bounds_.GetMaxX(); ++x)
    {
      buffered_data_[index].delta_wetness = 0;
      ++index;
    }
  }
}

void HexagonGrid::Draw( D3D11Renderer& gfx )
{
   int index = 0;
   float horizontal_distance;
   float vertical_distance;
   float hex_height = 1.0f;
   float hex_width = hex_height * sqrtf( 3.0f ) / 2.0f;
   horizontal_distance = hex_width;
   vertical_distance = hex_height * .75f;
   for ( size_t y = 0; y < bounds_.GetMaxY(); ++y )
   {
      int row_offset = y & 1;
      for ( size_t x = 0; x < bounds_.GetMaxX(); ++x )
      {
         tiles_[index].Draw( gfx, Vec2( ( x + row_offset / 2.0f )*horizontal_distance, y*vertical_distance ) );
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
      size_t index = y * bounds_.GetMaxX() + x;
      Vec2 points[6];
      Vec2 wetness_points[6];
      for (int i = 0; i < 6; ++i)
      {
        float angle_deg = (60.0f * i) + 30.0f;
        float angle_rad = glm::pi<float>() / 180.0f * angle_deg;
        points[i] = (Vec2(pos.x + hex_size_ * cos(angle_rad), pos.y + hex_size_ * sin(angle_rad)));
        wetness_points[i]=(Vec2(pos.x + hex_size_* tiles_[index].state->wetness() * cos(angle_rad), pos.y + hex_size_ * tiles_[index].state->wetness()* sin(angle_rad)));
      }
      Vec4u8 green ( 0, 255, 0, 0 );
      if ( tiles_[index].objects.size() != 0 )
      {
        green = Vec4u8( 255, 0, 0, 0 );
      }
      gfx.AddLine(Vec3(points[0],0), Vec3(points[1],0), green);
      gfx.AddLine(Vec3(points[1],0), Vec3(points[2],0), green);
      gfx.AddLine(Vec3(points[2],0), Vec3(points[3],0), green);
      gfx.AddLine(Vec3(points[3],0), Vec3(points[4],0), green);
      gfx.AddLine(Vec3(points[4],0), Vec3(points[5],0), green);
      gfx.AddLine(Vec3(points[5],0), Vec3(points[0],0), green);
      gfx.AddLine(Vec3(wetness_points[0], 0), Vec3(wetness_points[1], 0),green);
      gfx.AddLine(Vec3(wetness_points[1], 0), Vec3(wetness_points[2], 0),green);
      gfx.AddLine(Vec3(wetness_points[2], 0), Vec3(wetness_points[3], 0),green);
      gfx.AddLine(Vec3(wetness_points[3], 0), Vec3(wetness_points[4], 0),green);
      gfx.AddLine(Vec3(wetness_points[4], 0), Vec3(wetness_points[5], 0),green);
      gfx.AddLine(Vec3(wetness_points[5], 0), Vec3(wetness_points[0], 0),green);
    }
  }
}

void HexagonGrid::Update()
{
  for ( size_t y = 0; y < bounds_.GetMaxY(); ++y )
  {
    int row_offset = y & 1;
    int x_offset[2][6] = { { 1,0,-1,-1,-1,0 },{ 1,1,0,-1,0,1 } };
    int y_offset[2][6] = { { 0,-1,-1,0,1,1 },{ 0,-1,-1,0,1,1 } };
    for ( size_t x = 0; x < bounds_.GetMaxX(); ++x )
    {
      size_t index = y * bounds_.GetMaxX() + x;
      tiles_[index].Update(this);
    }
  }
}

void HexagonGrid::WetnessUpdate()
{
  // Wetness calculation pass
  for (size_t y = 0; y < bounds_.GetMaxY(); ++y)
  {
    int row_offset = y & 1;
    int x_offset[2][6] = { { 1,0,-1,-1,-1,0 },{ 1,1,0,-1,0,1 } };
    int y_offset[2][6] = { { 0,-1,-1,0,1,1 },{ 0,-1,-1,0,1,1 } };
    for (size_t x = 0; x < bounds_.GetMaxX(); ++x)
    {
      size_t index = y * bounds_.GetMaxX() + x;
      float delta_wetness = 0.f;
      for (int i = 0; i < 6; ++i)
      {
        size_t tile_x = x + x_offset[row_offset][i];
        size_t tile_y = y + y_offset[row_offset][i];
        if (bounds_.InBounds(tile_x,tile_y))
        {
          size_t target_index = (tile_y) * bounds_.GetMaxX() + (tile_x);
          float delta = tiles_[index].state->CalculateWaterTransition(tiles_[target_index].state)/ 60.0f;
          buffered_data_[target_index].delta_wetness -= delta;
          delta_wetness += delta;
        }
      }
      buffered_data_[index].delta_wetness += delta_wetness * Time::dt;
    }
  }
  // Apply delta wetness
  for (size_t y = 0; y < bounds_.GetMaxY(); ++y)
  {
    int row_offset = y & 1;
    int x_offset[2][6] = { { 1,0,-1,-1,-1,0 },{ 1,1,0,-1,0,1 } };
    int y_offset[2][6] = { { 0,-1,-1,0,1,1 },{ 0,-1,-1,0,1,1 } };
    for (size_t x = 0; x < bounds_.GetMaxX(); ++x)
    {
      size_t index = y * bounds_.GetMaxX() + x;
      tiles_[index].state->set_wetness(glm::clamp(tiles_[index].state->wetness() + buffered_data_[index].delta_wetness, 0.0f, 1.0f));
      buffered_data_[index].delta_wetness = 0.0f;
    }
  }
}
