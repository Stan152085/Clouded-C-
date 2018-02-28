#include "stdafx.h"
#include "renderer.h"
#include <D3D11.h>

D3D11Renderer::D3D11Renderer()
{
}

D3D11Renderer::~D3D11Renderer()
{
}

bool D3D11Renderer::Intialize(HWND window_handle, const math::Vec2u& screen_size)
{
  //buffer description
  DXGI_MODE_DESC buffer_desc;
  ZeroMemory(&buffer_desc, sizeof(DXGI_MODE_DESC));
  buffer_desc.Width = screen_size.x;
  buffer_desc.Height = screen_size.y;
  buffer_desc.RefreshRate.Numerator = 60; // 60hz
  buffer_desc.RefreshRate.Denominator = 1;
  buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // swap chain -> swapping back and front buffers
  DXGI_SWAP_CHAIN_DESC swap_chain_desc_;
  ZeroMemory(&swap_chain_desc_, sizeof(DXGI_SWAP_CHAIN_DESC));

  swap_chain_desc_.BufferDesc = buffer_desc;
  swap_chain_desc_.SampleDesc.Count = 1;
  swap_chain_desc_.SampleDesc.Quality = 0;
  swap_chain_desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc_.BufferCount = 1;
  swap_chain_desc_.OutputWindow = window_handle;
  swap_chain_desc_.Windowed = TRUE;
  swap_chain_desc_.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  D3D11CreateDeviceAndSwapChain(NULL,
                                D3D_DRIVER_TYPE_HARDWARE,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                D3D11_SDK_VERSION,
                                &swap_chain_desc_,
                                &swap_chain_,
                                &d3d11_device_,
                                NULL,
                                &d3d11_device_context_);

  ID3D11Texture2D* back_buffer;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
  d3d11_device_->CreateRenderTargetView(back_buffer, NULL, &render_target_view_);
  back_buffer->Release();
  d3d11_device_context_->OMSetRenderTargets(1, &render_target_view_, NULL);
  return true;
}

bool D3D11Renderer::Release()
{
  swap_chain_->Release();
  d3d11_device_->Release();
  d3d11_device_context_->Release();
  return true;
}

void D3D11Renderer::Draw()
{
  float color[4]{1.0f ,1.0f ,1.0f ,1.0f};
  d3d11_device_context_->ClearRenderTargetView(render_target_view_, color);
  swap_chain_->Present(0, 0);
}
