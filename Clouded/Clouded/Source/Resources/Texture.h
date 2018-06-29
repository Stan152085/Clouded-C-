#pragma once
#include <stdafx.h>

struct GPUTextureResource;
using GPUTextureResourceHandle = std::shared_ptr<GPUTextureResource>;

namespace resources
{
  class Texel;

  // simple texture width 
  class Texture
  {
  public:
    Texture(
      const int width, const int height,
      const int components, const unsigned char* data);
    ~Texture();

    const int width() const
    {
      return width_;
    }
    const int height() const
    {
      return height_;
    }
    const Texel* pixels() const
    {
      return pixels_;
    }

    GPUTextureResourceHandle* gpu_handle()
    {
      return &gpu_handle_;
    }
  private:
    int width_, height_;
    Texel* pixels_;
    GPUTextureResourceHandle gpu_handle_;
  };
}