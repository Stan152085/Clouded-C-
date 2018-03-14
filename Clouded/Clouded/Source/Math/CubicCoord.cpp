#include "stdafx.h"
#include "CubicCoord.h"

CubicCoord::CubicCoord(Vec2 pos, float size)
{
  pos /= size;
  x = ((pos.x * sqrtf(3.0f) / 3.0f) - pos.y / 3.0f);
  z = pos.y * 2.f / 3.f;
  y = -x - z;
}

void CubicCoord::Round()
{
  float rx = round(x);
  float ry = round(y);
  float rz = round(z);

  float x_diff = fabs(rx - x);
  float y_diff = fabs(ry - y);
  float z_diff = fabs(rz - z);

  if (x_diff > y_diff || x_diff > z_diff)
  {
    x = -ry - rz;
    y = ry;
    z = rz;
  }
  else if (y_diff > z_diff)
  {
    x = rx;
    y = -rx - rz;
    z = rz;
  }
  else
  {
    x = rx;
    y = ry;
    z = -rx - ry;
  }
}
