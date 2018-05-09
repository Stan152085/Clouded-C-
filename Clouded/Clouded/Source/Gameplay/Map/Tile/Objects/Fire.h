#pragma once
#include "IObject.h"
class Fire : public IObject
{
  virtual void Update() override;
  virtual void Draw( float height ) override;
};