#include "stdafx.h"
#include "Texture.h"
#include "Texel.h"

resources::Texture::Texture(
  const int width, const int height,
  const int components, const unsigned char* data)
  :
  width_(width),
  height_(height),
  pixels_(new Texel[width * height]),
  gpu_handle_(nullptr)
{
  if (components == 1)
  {
    size_t idx = 0;
    for (int y = 0; y < height_; ++y)
    {
      for (int x = 0; x < width_; ++x, ++idx)
      {
        pixels_[idx] = Texel(data[idx]);
      }
    }
  }
  else if (components == 2)
  {
    size_t pixel_idx = 0;
    size_t data_idx = 0;
    for (int y = 0; y < height_; ++y)
    {
      for (int x = 0; x < width_; ++x, ++pixel_idx, data_idx += 2)
      {
        pixels_[pixel_idx] = Texel(data[data_idx], data[data_idx + 1]);
      }
    }
  }
  else if (components == 3)
  {
    size_t pixel_idx = 0;
    size_t data_idx = 0;
    for (int y = 0; y < height_; ++y)
    {
      for (int x = 0; x < width_; ++x, ++pixel_idx, data_idx += 3)
      {
        pixels_[pixel_idx] = Texel(
          data[data_idx],
          data[data_idx + 1],
          data[data_idx + 2]);
      }
    }
  }
  else if (components == 4)
  {
    size_t pixel_idx = 0;
    size_t data_idx = 0;
    for (int y = 0; y < height_; ++y)
    {
      for (int x = 0; x < width_; ++x, ++pixel_idx, data_idx += 4)
      {
        pixels_[pixel_idx] = Texel(
            data[data_idx],
            data[data_idx + 1],
            data[data_idx + 2],
            data[data_idx + 3]);
      }
    }
  }
  else
  {
    assert(false && "Unexpected number of components detected when constructing a texture");
  }
}

resources::Texture::~Texture()
{
  delete[] pixels_;
}
