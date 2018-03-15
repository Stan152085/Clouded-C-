#include "stdafx.h"
#include "DebugRenderer.h"
#include "Renderer.h"

D3D11Renderer* DebugRenderer::renderer_ = nullptr;
void DebugRenderer::DrawLine(const Vec3& from, const Vec3& to)
{
  if (renderer_ == nullptr)
  {
    return;
  }
  renderer_->AddLine(from, to);
}

void DebugRenderer::set_renderer(D3D11Renderer* const renderer)
{
  renderer_ = renderer;
}
