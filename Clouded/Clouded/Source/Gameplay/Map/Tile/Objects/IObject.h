#pragma once
#include "Core/Transform.h"
class IObject
{
public:
  virtual void Update() = 0;
  virtual void Draw(float height) = 0;

  Transform transform;
};