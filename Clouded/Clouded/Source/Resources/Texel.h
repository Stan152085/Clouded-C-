#pragma once
#include <stdint.h>

namespace resources
{
  // pixel class with ARGB8888 encoding
  class Texel
  {
  public:
    Texel()
      :
      color(0)
    {

    }
    Texel(unsigned char gray_scale)
      :
      a(0xff),
      r(gray_scale),
      g(gray_scale),
      b(gray_scale)
    {}
    Texel(unsigned char alpha, unsigned char gray_scale)
      :
      a(alpha),
      r(gray_scale),
      g(gray_scale),
      b(gray_scale)
    {}
    Texel(unsigned char r, unsigned char g, unsigned char b)
      :
      a(0xff),
      r(r),
      g(g),
      b(b)
    {}
    Texel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
      :
      a(a),
      r(r),
      g(g),
      b(b)
    {}

  public:
    union
    {
      int32_t color;
      struct
      {
        unsigned char a, r, g, b;
      };
    };
  };
}
