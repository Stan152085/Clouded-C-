#pragma once
#include "Math/math_defines.h"

namespace resources
{
  struct Vertex
  {
    Vec3 pos;
    Vec3 norm;
    Vec2 uv;
    Vec4 tang; // xyz is tangent, w is handedness
  };
}