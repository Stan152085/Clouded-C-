#include "DebugRenderer.h"
#include "Renderer.h"

D3D11Renderer* DebugRenderer::renderer_ = nullptr;
void DebugRenderer::DrawLine(Vec3 from, Vec3 to)
{
  if (renderer_ == nullptr)
  {
    return;
  }
  renderer_->AddLine(from, to);
}
