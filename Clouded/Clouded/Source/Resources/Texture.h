#pragma once

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

  private:
    int width_, height_;
    Texel* pixels_;
  };
}