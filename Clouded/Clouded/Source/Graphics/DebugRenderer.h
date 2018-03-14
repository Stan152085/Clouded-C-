#pragma once
#include "stdafx.h"

class D3D11Renderer;

class DebugRenderer
{
public:
  DebugRenderer() = default;
  void DrawLine(Vec3 from, Vec3 to);

private:
  friend D3D11Renderer;
  static D3D11Renderer* renderer_;
};