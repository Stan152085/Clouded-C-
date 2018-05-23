#pragma once
#include "stdafx.h"
#include "GPUModel.h"
#include <D3D11.h>
#include <queue>
#include <OpenVr\openvr.h>

class Input;
class Camera;

namespace resources
{
  struct Vertex;
  class Model;
}

enum struct RenderModes
{
  kSolid,
  kWireframe
};

// Feel free to rename
struct DrawCall   
{
   ModelHandle handle;
   Mat44 world;
};

class D3D11Renderer
{

public:
	D3D11Renderer();
	~D3D11Renderer();

	bool Intialize(HWND window_handle, const Vec2u& screen_size, vr::IVRSystem* vr_system);

  bool Release();
  void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void AddLine(const Vec3& from, const Vec3& to, const Vec4u8& color);

  void Clear();
	void Present(Input* input);
  void SetCamera(Camera* cam);

  
  void ReleaseFromGPU(ModelHandle handle);
  ModelHandle PushToGPU(const resources::Model& model);

  void AddToDrawQueue(ModelHandle handle, const Mat44& world);
  void SetRenderState(RenderModes mode);
private:
  enum struct RenderTargets
  {
    kLeftEye,
    kRightEye,
    kDebugCamera
  };
  DrawCall DrawModel(DrawCall model, RenderTargets render_target);
  void DrawDebug(RenderTargets render_target);
  void GetViewProjectionMatrix(RenderTargets& target, Mat44& view, Mat44& projection);

  class RenderTexture
  {
  public:
    RenderTexture();
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

  void RenderDrawQueue(Input* input);

  void ReadShader(const char* shader_name, std::string& buffer);
  void UpdatateHmdPose();
	IDXGISwapChain* swap_chain_;
	ID3D11Device* d3d11_device_;
	ID3D11DeviceContext* d3d11_device_context_;

  // debug window
	ID3D11RenderTargetView* debug_render_target_view_;
  ID3D11DepthStencilView* debug_depth_stencil_view_;
  D3D11_VIEWPORT debug_viewport_;
  ID3D11Texture2D* debug_depth_stencil_buffer_;

  /*primitive / model resources*/
  ID3D11Buffer* vert_buffers_;
  ID3D11Buffer* index_buffer_;
  ID3D11Buffer* line_buffer_;

  // vr
  D3D11_VIEWPORT vr_viewport_;
  ID3D11DepthStencilView* vr_depth_stencil_view_;
  ID3D11Texture2D* vr_depth_stencil_buffer_;



  /*shader program resources*/
  ID3D11VertexShader* vs_;
  ID3D11PixelShader* ps_;
  ID3D11VertexShader* vs_debug_;
  ID3D11PixelShader* ps_debug_;

  ID3D11InputLayout* vert_layout_;
  ID3D11InputLayout* vert_layout_debug_;
  /*constant buffers*/
  ID3D11Buffer* cb_per_object_buffer_;

  /*render states*/
  ID3D11RasterizerState* wireframe_;
  ID3D11RasterizerState* solid_;

  D3D11_INPUT_ELEMENT_DESC layout[3] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
   // { "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  D3D11_INPUT_ELEMENT_DESC debug_layout[2] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  Camera* current_camera_;
  
  /*vr stuff*/
  RenderTexture* left_eye_;
  RenderTexture* right_eye_;

  float clear_color_[4];
  vr::IVRSystem* vr_system_;
  std::queue<DrawCall> draw_queue_;
  vr::TrackedDevicePose_t tracked_device_pose_[vr::k_unMaxTrackedDeviceCount];
  Mat44 hmd_pose;
  Mat44 device_pose[vr::k_unMaxTrackedDeviceCount];
  vr::IVRRenderModels* render_models;
};




