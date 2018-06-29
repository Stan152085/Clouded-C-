#pragma once
#include <stdafx.h>

enum struct lightTypes : int
{
  kDirectional,
  kPoint,
  kSpot
};

struct Light
{
  Vec4 position;
  Vec4 direction;
  Vec4 color;
  float spotangle;
  float const_attenuation;
  float linear_attenuation;
  float quad_attenuation;
  int type;
private:
  int pad_[3];
};

const int i = sizeof Light;