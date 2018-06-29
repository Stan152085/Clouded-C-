#pragma once
#include <stdafx.h>

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11Device;


class VRRenderTexture
{
public:
  VRRenderTexture();
  bool Create(ID3D11Device*, uint32_t, uint32_t);
  void Release();

  ID3D11Texture2D* texture();
  ID3D11RenderTargetView* render_target();
  ID3D11ShaderResourceView* shader_resource();
private:
  ID3D11Texture2D * texture_;
  ID3D11RenderTargetView* render_target_;
  ID3D11ShaderResourceView* shader_resource_;
};