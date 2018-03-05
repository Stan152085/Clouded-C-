#pragma once
#include "stdafx.h"
#include <D3D11.h>

namespace resources
{
  struct Vertex;
}

class D3D11Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

	bool Intialize(HWND window_handle, const Vec2u& screen_size);
  bool Release();
  void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void Draw();

private:
  void ReadShader(const char* shader_name, std::vector<char> buffer);

	IDXGISwapChain* swap_chain_;
	ID3D11Device* d3d11_device_;
	ID3D11DeviceContext* d3d11_device_context_;
	ID3D11RenderTargetView* render_target_view_;
  ID3D11Buffer* triangle_vert_buffer_;

  /*shader program resources*/
  ID3D11VertexShader* vs_;
  ID3D11PixelShader* ps_;
  ID3D10Blob* vs_buffer_;
  ID3D10Blob* ps_buffer_;
  ID3D11InputLayout* vert_layout_;

  D3D11_INPUT_ELEMENT_DESC layout[1] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  float clear_color_[4];
};