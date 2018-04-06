#include "stdafx.h"
#include "DebugRenderer.h"
#include "Renderer.h"

D3D11Renderer* DebugRenderer::renderer_ = nullptr;
void DebugRenderer::DrawLine(const Vec3& from, const Vec3& to, const Vec4u8& color)
{
  if (renderer_ == nullptr)
  {
    return;
  }
  renderer_->AddLine(from, to, color);
}

void DebugRenderer::set_renderer(D3D11Renderer* const renderer)
{
  renderer_ = renderer;
}
