#pragma once
#include "stdafx.h"

class D3D11Renderer;

class DebugRenderer
{
public:
  DebugRenderer() = default;
  static void DrawLine(const Vec3& from, const Vec3& to, const Vec4u8& color);
  void set_renderer(D3D11Renderer* const renderer);

private:
  static D3D11Renderer* renderer_;
};