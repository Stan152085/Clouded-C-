#pragma once
#include "stdafx.h"
#include <D3D11.h>

class D3D11Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

	bool Intialize(HWND window_handle, const math::Vec2u& screen_size);
  bool Release();
	void Draw();

private:
	IDXGISwapChain* swap_chain_;
	ID3D11Device* d3d11_device_;
	ID3D11DeviceContext* d3d11_device_context_;
	ID3D11RenderTargetView* render_target_view_;
};