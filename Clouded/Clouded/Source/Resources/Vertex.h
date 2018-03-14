#pragma once
#include "Math/math_defines.h"

namespace resources
{
  struct Vertex
  {
    Vec3 pos;
    Vec3 norm;
    Vec4 tang; // xyz is tangent, w is handedness
    Vec2 uv;
  };
}