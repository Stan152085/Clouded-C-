#include "stdafx.h"
#include "vr_render_texture.h"
#include <d3d11.h>

VRRenderTexture::VRRenderTexture() :
  texture_(nullptr),
  render_target_(nullptr),
  shader_resource_(nullptr)
{
}

bool VRRenderTexture::Create(ID3D11Device* d3d11_device, uint32_t width, uint32_t height)
{
  HRESULT result;

  D3D11_TEXTURE2D_DESC texture_desc{};
  texture_desc.Width = width;
  texture_desc.Height = height;
  texture_desc.MipLevels = 1;
  texture_desc.ArraySize = 1;
  texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  texture_desc.SampleDesc.Count = 1;
  texture_desc.Usage = D3D11_USAGE_DEFAULT;
  texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

  result = d3d11_device->CreateTexture2D(&texture_desc, NULL, &texture_);
  if (FAILED(result))
  {
    printf("failed to create texture");
    return false;
  }

  D3D11_RENDER_TARGET_VIEW_DESC render_target_desc{};
  render_target_desc.Format = texture_desc.Format;
  render_target_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  render_target_desc.Texture2D.MipSlice = 0;

  result = d3d11_device->CreateRenderTargetView(texture_, &render_target_desc, &render_target_);
  if (FAILED(result))
  {
    printf("failed to create render target");
    return false;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_desc{};
  shader_resource_desc.Format = texture_desc.Format;
  shader_resource_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shader_resource_desc.Texture2D.MostDetailedMip = 0;
  shader_resource_desc.Texture2D.MipLevels = 1;

  result = d3d11_device->CreateShaderResourceView(texture_, &shader_resource_desc, &shader_resource_);
  if (FAILED(result))
  {
    printf("failed to create shader resource");
    return false;
  }

  return true;
}

void VRRenderTexture::Release()
{
  texture_->Release();
  render_target_->Release();
  shader_resource_->Release();

  texture_ = nullptr;
  render_target_ = nullptr;
  shader_resource_ = nullptr;
}

ID3D11Texture2D * VRRenderTexture::texture()
{
  return texture_;
}

ID3D11RenderTargetView *  VRRenderTexture::render_target()
{
  return render_target_;
}

ID3D11ShaderResourceView * VRRenderTexture::shader_resource()
{
  return shader_resource_;
}