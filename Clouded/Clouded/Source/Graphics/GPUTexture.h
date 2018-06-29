#pragma once
#include <stdafx.h>

struct ID3D11ShaderResourceView;
struct GPUTextureResource
{
  GPUTextureResource(ID3D11ShaderResourceView* texture) :
    texture_(texture)
  {

  }

   ID3D11ShaderResourceView* texture_;
};

using GPUTextureResourceHandle = std::shared_ptr<GPUTextureResource>;