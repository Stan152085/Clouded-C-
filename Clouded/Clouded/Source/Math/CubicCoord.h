#pragma once
#include "glm\fwd.hpp"
using Vec2 = glm::f32vec2;

struct CubicCoord
{
  CubicCoord(Vec2 pos, float size);
  void Round();

public:
  union
  {
    float f[3];
    struct
    {
      float x, y, z;
    };
  };
};