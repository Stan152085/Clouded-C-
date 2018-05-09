#pragma once
#include "IObject.h"
class Tree : public IObject
{
  virtual void Update() override;
  virtual void Draw( float height ) override;
};