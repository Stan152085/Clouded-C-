#pragma once
#include "stdafx.h"
#include <D3D11.h>

class Camera;

namespace resources
{
  struct Vertex;
  class Model;
}

struct GPUModel
{
  GPUModel(ID3D11Buffer* const vert_buffer, ID3D11Buffer* const idx_buffer, const std::vector<size_t> num_idices, const std::vector<size_t> vert_offsets) :
    vert_buffer_(vert_buffer),
    idx_buffer_(idx_buffer),
    num_idices_(num_idices),
    vert_offsets_(vert_offsets)
  {

  }

  ID3D11Buffer* const vert_buffer_;
  ID3D11Buffer* const idx_buffer_;
  const std::vector<size_t> num_idices_; //number of indices per mesh
  const std::vector<size_t> vert_offsets_; //offset of the first vertex for each mesh
};

using ModelHandle = std::shared_ptr<GPUModel>;

enum struct RenderModes
{
  kSolid,
  kWireframe
};


class D3D11Renderer
{

public:
	D3D11Renderer();
	~D3D11Renderer();

	bool Intialize(HWND window_handle, const Vec2u& screen_size);

  bool Release();
  void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void AddLine(const Vec3& from, const Vec3& to );

  void Clear();
	void Present();
  void SetCamera(Camera* cam);

  void ReleaseFromGPU(ModelHandle handle);
  ModelHandle PushToGPU(const resources::Model& model);

  void DrawModel(ModelHandle model);
  void SetRenderState(RenderModes mode);
private:
  void ReadShader(const char* shader_name, std::vector<char>& buffer);

	IDXGISwapChain* swap_chain_;
	ID3D11Device* d3d11_device_;
	ID3D11DeviceContext* d3d11_device_context_;
	ID3D11RenderTargetView* render_target_view_;
  ID3D11DepthStencilView* depth_stencil_view_;
  ID3D11Texture2D* depth_stencil_buffer_;

  /*primitive / model resources*/
  ID3D11Buffer* vert_buffers_;
  ID3D11Buffer* index_buffer_;

  ID3D11Buffer* line_buffer_;
  /*shader program resources*/
  ID3D11VertexShader* vs_;
  ID3D11PixelShader* ps_;
  ID3D11InputLayout* vert_layout_;

  /*constant buffers*/
  ID3D11Buffer* cb_per_object_buffer_;

  /*render states*/
  ID3D11RasterizerState* wireframe_;
  ID3D11RasterizerState* solid_;

  D3D11_INPUT_ELEMENT_DESC layout[4] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };
  Camera* current_camera_;
  
  float clear_color_[4];
};

