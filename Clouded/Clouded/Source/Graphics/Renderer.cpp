#include "stdafx.h"
#include "renderer.h"
#include "fstream"
#include "Resources\Model.h"
#include "Resources\Mesh.h"
#include "Core\Camera.h"
#include "DebugRenderer.h"
#include "resources\Texel.h"
#include "Input\Input.h"
#include "cb_buffers.h"
#include "vr_render_texture.h"
#include "Lights.h"
#include "Resources/Texture.h"
#include "GPUModel.h"
#include "GPUTexture.h"


Mat44 VrMat34ToMat44(const vr::HmdMatrix34_t &matPose)
{
  Mat44 matrixObj(
    matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
    matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
    matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
    matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
  );
  return matrixObj;
}

Mat44 VrMat44ToMat44(const vr::HmdMatrix44_t &matPose)
{
  Mat44 result{ matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], matPose.m[3][0],
    matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], matPose.m[3][1],
    matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], matPose.m[3][2],
    matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], matPose.m[3][3] };
  return result;
}

class VRCamera
{
public:
  Mat44 inv_head_pose_;
  Mat44 left_eye_projection_;
  Mat44 left_eye_view_;

  Mat44 right_eye_projection_;
  Mat44 right_eye_view_;

  Mat44 view_left_;
  Mat44 view_right_;

private:

};

VRCamera vr_cam;

struct DebugVertex
{
  Vec3 position;
  Vec4u8 color;
};

constexpr unsigned int max_line_count_ = 500;
uint32_t current_line_count_ = 0;
DebugVertex line_vertices[max_line_count_ * 2];

cb_per_object cb_per_obj;
// Camera cam;

//-------------------------------------------------------------------------------------------------
D3D11Renderer::D3D11Renderer() :
  clear_color_{ 0.0f ,0.0f ,0.0f ,0.0f }
{
}

//-------------------------------------------------------------------------------------------------
D3D11Renderer::~D3D11Renderer()
{
}

//-------------------------------------------------------------------------------------------------
bool D3D11Renderer::Intialize(HWND window_handle, const Vec2u& screen_size, vr::IVRSystem* vr_system)
{
  vr_system_ = vr_system;
 
  /*DX11 intialization*/
  HRESULT result;
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

  result = D3D11CreateDeviceAndSwapChain(NULL,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    D3D11_CREATE_DEVICE_DEBUG,
    NULL,
    NULL,
    D3D11_SDK_VERSION,
    &swap_chain_desc_,
    &swap_chain_,
    &d3d11_device_,
    NULL,
    &d3d11_device_context_);
  if (FAILED(result))
  {
    printf("Failed to create swap chain \n");
    return false;
  }

  ID3D11Texture2D* back_buffer;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
  result = d3d11_device_->CreateRenderTargetView(back_buffer, NULL, &debug_render_target_view_);
  if (FAILED(result))
  {
    printf("Failed to create render target");
    return false;
  }

  back_buffer->Release();
  D3D11_TEXTURE2D_DESC depth_stencil_desc{};

  // desc for the debug depth stencil
  depth_stencil_desc = {};
  depth_stencil_desc.Width = screen_size.x;
  depth_stencil_desc.Height = screen_size.y;
  depth_stencil_desc.MipLevels = 1;
  depth_stencil_desc.ArraySize = 1;
  depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_stencil_desc.SampleDesc.Count = 1;
  depth_stencil_desc.SampleDesc.Quality = 0;
  depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
  depth_stencil_desc.CPUAccessFlags = 0;
  depth_stencil_desc.MiscFlags = 0;
  result = d3d11_device_->CreateTexture2D(&depth_stencil_desc, NULL, &debug_depth_stencil_buffer_);
  if (FAILED(result))
  {
    printf("Failed to create texture \n");
    return false;
  }

  result = d3d11_device_->CreateDepthStencilView(debug_depth_stencil_buffer_, NULL, &debug_depth_stencil_view_);
  if (FAILED(result))
  {
    printf("Failed to create depth stencil view \n");
    return false;
  }


  /*set up shaders and vertex buffers to render a triangle*/
  std::string vs_buf;
  std::string ps_buf;
  ReadShader("VertexShader", vs_buf);
  result = d3d11_device_->CreateVertexShader(vs_buf.data(), vs_buf.length(), NULL, &vs_);
  if (FAILED(result))
  {
    printf("Failed to create vertex shader \n");
    return false;
  }

  ReadShader("PixelShader", ps_buf);
  result = d3d11_device_->CreatePixelShader(ps_buf.data(), ps_buf.size(), NULL, &ps_);
  if (FAILED(result))
  {
    printf("Failed to create pixel shader \n");
    return false;
  }

  // create input layout and set primitive topology
  result = d3d11_device_->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vs_buf.data(), vs_buf.size(), &vert_layout_);
  if (FAILED(result))
  {
    printf("Failed to create input layout \n");
    return false;
  }

  // setup the debug shaders
  ReadShader("debug_vertex_shader", vs_buf);
  result = d3d11_device_->CreateVertexShader(vs_buf.data(), vs_buf.length(), NULL, &vs_debug_);
  if (FAILED(result))
  {
    printf("Failed to create vertex shader \n");
    return false;
  }

  ReadShader("debug_pixel_shader", ps_buf);
  result = d3d11_device_->CreatePixelShader(ps_buf.data(), ps_buf.size(), NULL, &ps_debug_);

  if (FAILED(result))
  {
    printf("Failed to create pixel shader \n");
    return false;
  }

  result = d3d11_device_->CreateInputLayout(debug_layout,
                                            sizeof(debug_layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
                                            vs_buf.data()
                                            , vs_buf.size(),
                                            &vert_layout_debug_);

  /*create line buffer*/
  D3D11_BUFFER_DESC line_buffer_desc;
  ZeroMemory(&line_buffer_desc, sizeof(D3D11_BUFFER_DESC));
  line_buffer_desc.ByteWidth = max_line_count_ * 2 * sizeof(DebugVertex);
  line_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  line_buffer_desc.CPUAccessFlags = NULL;
  line_buffer_desc.MiscFlags = NULL;
  line_buffer_desc.StructureByteStride = NULL;
  line_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  result = d3d11_device_->CreateBuffer(&line_buffer_desc, NULL, &line_buffer_);
  if (FAILED(result))
  {
    printf("Failed create line buffer \n");
    return false;
  }

  // create the viewport for rendering the debug window
  debug_viewport_ = {};
  debug_viewport_.TopLeftX = 0;
  debug_viewport_.TopLeftY = 0;
  debug_viewport_.MinDepth = 0.0f;
  debug_viewport_.MaxDepth = 1.0f;
  debug_viewport_.Width = (float)screen_size.x;
  debug_viewport_.Height = (float)screen_size.y;

  // create constant buffers
  D3D11_BUFFER_DESC cb_buffer_desc{};
  cb_buffer_desc.ByteWidth = sizeof(cb_per_object);
  cb_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cb_buffer_desc.CPUAccessFlags = NULL;
  cb_buffer_desc.MiscFlags = NULL;
  cb_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  result = d3d11_device_->CreateBuffer(&cb_buffer_desc, NULL, &cb_per_object_buffer_);
  if (FAILED(result))
  {
    printf("Failed create per object buffer \n");
    return false;
  }

  cb_buffer_desc = {};
  cb_buffer_desc.ByteWidth = sizeof(cb_material_per_mesh);
  cb_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cb_buffer_desc.CPUAccessFlags = NULL;
  cb_buffer_desc.MiscFlags = NULL;
  cb_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  result = d3d11_device_->CreateBuffer(&cb_buffer_desc, NULL, &cb_material_per_object_buffer_);
  if (FAILED(result))
  {
    printf("Failed create material per object buffer \n");
    return false;
  }

  cb_buffer_desc = {};
  cb_buffer_desc.ByteWidth = sizeof(cb_lights_per_frame);
  cb_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cb_buffer_desc.CPUAccessFlags = NULL;
  cb_buffer_desc.MiscFlags = NULL;
  cb_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  result = d3d11_device_->CreateBuffer(&cb_buffer_desc, NULL, &cb_lights_per_draw_buffer_);
  if (FAILED(result))
  {
    printf("Failed create lights per draw buffer \n");
    return false;
  }


  // create rasterizer states
  D3D11_RASTERIZER_DESC wireframe_desc{};
  wireframe_desc.FillMode = D3D11_FILL_WIREFRAME;
  wireframe_desc.CullMode = D3D11_CULL_NONE;
  result = d3d11_device_->CreateRasterizerState(&wireframe_desc, &wireframe_);
  if (FAILED(result))
  {
    printf("Failed create wireframe rasterizer state \n");
    return false;
  }

  D3D11_RASTERIZER_DESC solid_desc {};
  solid_desc.FillMode = D3D11_FILL_SOLID;
  solid_desc.CullMode = D3D11_CULL_BACK;
  result = d3d11_device_->CreateRasterizerState(&solid_desc, &solid_);
  if (FAILED(result))
  {
    printf("Failed create solid rasterizer state \n");
    return false;
  }
  d3d11_device_context_->RSSetState(solid_);

  D3D11_SAMPLER_DESC sampler_desc{};
  sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampler_desc.MipLODBias = 0.0f;
  sampler_desc.MaxAnisotropy = 1;
  sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampler_desc.BorderColor[0] = 1.0f;
  sampler_desc.BorderColor[1] = 1.0f;
  sampler_desc.BorderColor[2] = 1.0f;
  sampler_desc.BorderColor[3] = 1.0f;
  sampler_desc.MinLOD = -FLT_MAX;
  sampler_desc.MaxLOD = FLT_MAX;

  result = d3d11_device_->CreateSamplerState(&sampler_desc, &default_sampler_);
  if (FAILED(result))
  {
    printf("failed to create default texture sampler.");
    return false;
  }


  if (vr_system_ != nullptr)
  {
    vr::EVRInitError err;
    render_models = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &err);
    if (!render_models)
    {
      printf("unable to get render models \n");
    }
    uint32_t eye_width;
    uint32_t eye_height;
    vr_system_->GetRecommendedRenderTargetSize(&eye_width, &eye_height);
    left_eye_ = new VRRenderTexture();
    right_eye_ = new VRRenderTexture();

    left_eye_->Create(d3d11_device_, eye_width, eye_height);
    right_eye_->Create(d3d11_device_, eye_width, eye_height);

    D3D11_TEXTURE2D_DESC depth_stencil_desc{};
    depth_stencil_desc.Width = eye_width;
    depth_stencil_desc.Height = eye_height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;
    d3d11_device_->CreateTexture2D(&depth_stencil_desc, NULL, &vr_depth_stencil_buffer_);
    d3d11_device_->CreateDepthStencilView(vr_depth_stencil_buffer_, NULL, &vr_depth_stencil_view_);

    // create the viewport for rendering vr
    vr_viewport_ = {};
    vr_viewport_.TopLeftX = 0;
    vr_viewport_.TopLeftY = 0;
    vr_viewport_.MinDepth = 0.0f;
    vr_viewport_.MaxDepth = 1.0f;
    vr_viewport_.Width = (float)eye_width;
    vr_viewport_.Height = (float)eye_height;
  }
  return true;
}

//-------------------------------------------------------------------------------------------------
bool D3D11Renderer::Release()
{
  swap_chain_->Release();
  d3d11_device_->Release();

  d3d11_device_context_->Release();
  vr_depth_stencil_buffer_->Release();
  vr_depth_stencil_view_->Release();
  left_eye_->Release();
  right_eye_->Release();

  debug_render_target_view_->Release();
  debug_depth_stencil_buffer_->Release();
  debug_depth_stencil_view_->Release();
  ps_debug_->Release();
  vs_debug_->Release();
  vert_layout_debug_->Release();
  
  line_buffer_->Release();
  vs_->Release();
  ps_->Release();
  vert_layout_->Release();
  cb_per_object_buffer_->Release();
  cb_lights_per_draw_buffer_->Release();
  cb_material_per_object_buffer_->Release();
  wireframe_->Release();
  solid_->Release();
  return true;
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  float mult = 1 / 255;
  clear_color_[0] = { mult * r };
  clear_color_[1] = { mult * g };
  clear_color_[2] = { mult * b };
  clear_color_[3] = { mult * a };
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::AddLine(const Vec3& from, const Vec3& to, const Vec4u8& color)
{
  if (current_line_count_ >= max_line_count_)
  {
    printf("exeeding the maxim number of debug lines. some lines will not be drawn");
    return;
  }
  line_vertices[current_line_count_ * 2] = { from, color };
  line_vertices[current_line_count_ * 2 + 1] = { to, color };
  ++current_line_count_;
}

void D3D11Renderer::AddDirectionalLight(const Vec3 & direction, const resources::Texel & color)
{
  Light l;
  l.position = Vec4(0.0f);
  l.direction = Vec4(glm::normalize(direction), 1.0f);
  l.color = color.GetNormalized();
  l.type = (int)lightTypes::kDirectional;
  l.const_attenuation = 1.0f;
  l.linear_attenuation = 0.08f;
  l.quad_attenuation = 0.0f;
  l.spotangle = 0.0f;
  lights_.push_back(l);
}


void D3D11Renderer::AddPointLight(const Vec3 & position, const resources::Texel& color)
{
  Light l;
  l.position = Vec4(position, 1.0f);
  l.direction = Vec4(0,0,0,1);
  l.color = color.GetNormalized();
  l.type = (int)lightTypes::kPoint;
  l.const_attenuation = 0.5f;
  l.linear_attenuation = 0.02f;
  l.quad_attenuation = 0.0f;
  l.spotangle = 0.0f;
  lights_.push_back(l);
}

void D3D11Renderer::AddSpotLight(const Vec3& position, const Vec3& direction, const float& spot_angle, const resources::Texel & color)
{
  Light l;
  l.position = Vec4(position, 1.0f);
  l.direction = Vec4(glm::normalize(direction), 1.0f);
  l.color = color.GetNormalized();
  l.type = (int)lightTypes::kSpot;
  l.const_attenuation = 0.5f;
  l.linear_attenuation = 0.02f;
  l.quad_attenuation = 0.0f;
  l.spotangle = glm::radians(spot_angle);
  lights_.push_back(l);
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::Clear()
{
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::Present(Input* input)
{
  if (vr_system_)
  {
    UpdatateHmdPose();
  }

  d3d11_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  RenderDrawQueue(input);
   
  swap_chain_->Present(0, 0);
  current_line_count_ = 0;

  if (vr_system_ != nullptr)
  {
    if (!vr::VRCompositor())
    {
      printf("no compositor");
    }
    vr::Texture_t left = { left_eye_->texture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
    vr::EVRCompositorError error1 = vr::VRCompositor()->Submit(vr::Eye_Left, &left);
    vr::Texture_t right = { right_eye_->texture(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
    vr::VRCompositor()->Submit(vr::Eye_Right, &right);
    if (error1)
    {
      printf("error is: %d \n", error1);
    }
  }
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::SetCamera(Camera * cam)
{
  current_camera_ = cam;
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::ReleaseFromGPU(GPUModelResourceHandle handle)
{
  handle->idx_buffer_->Release();
  handle->vert_buffer_->Release();
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::ReleaseFromGPU(GPUTextureResourceHandle handle)
{
  handle->texture_->Release();
}

//-------------------------------------------------------------------------------------------------
bool D3D11Renderer::PushToGPU( resources::Model& model)
{
  ID3D11Buffer* vertex_buffer;
  ID3D11Buffer* index_buffer;

  std::vector<size_t> num_idices(model.meshes().size());
  std::vector<size_t> vert_offsets(model.meshes().size());
  size_t vertex_total = 0;
  size_t indices_total = 0;
  for (size_t i = 0; i < model.meshes().size(); ++i)
  {
    indices_total += model.meshes()[i].indices().size();
    i > 0 ? num_idices[i] = indices_total - num_idices[i - 1] : num_idices[i] = indices_total;

    i > 0 ? vert_offsets[i] = vertex_total - vert_offsets[i - 1] : vert_offsets[i] = vertex_total;
    vertex_total += model.meshes()[i].vertices().size();
  }

  D3D11_BUFFER_DESC vertex_buffer_desc{};
  vertex_buffer_desc.ByteWidth = (uint32_t)(vertex_total * sizeof(resources::Vertex));
  vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

  D3D11_BUFFER_DESC index_buffer_desc{};
  index_buffer_desc.ByteWidth = (uint32_t)(indices_total * sizeof(unsigned short));
  index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

  resources::Vertex* vertex_array_base = new resources::Vertex[vertex_total];
  unsigned short* index_array_base = new unsigned short[indices_total];

  resources::Vertex* vertex_it = vertex_array_base;
  unsigned short* index_it = index_array_base;

  for (const resources::Mesh& mesh : model.meshes())
  {
    memcpy(vertex_it, mesh.vertices().data(), mesh.vertices().size() * sizeof resources::Vertex);
    memcpy(index_it, mesh.indices().data(), mesh.indices().size() * sizeof(unsigned short));

    vertex_it += mesh.vertices().size();
    index_it += mesh.indices().size();
  }
  D3D11_SUBRESOURCE_DATA vertex_data{};
  vertex_data.pSysMem = vertex_array_base;
  d3d11_device_->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);

  D3D11_SUBRESOURCE_DATA index_data{};
  index_data.pSysMem = index_array_base;
  d3d11_device_->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);

  delete[] vertex_array_base;
  delete[] index_array_base;

  GPUModelResourceHandle* handle = model.gpu_handle();
  *handle = std::make_shared<GPUModelResource>(vertex_buffer, index_buffer, num_idices, vert_offsets);
  return true;
}

//-------------------------------------------------------------------------------------------------
bool D3D11Renderer::PushToGPU(resources::Texture& texture)
{
  D3D11_TEXTURE2D_DESC texture_desc{};
  texture_desc.Width = texture.width();
  texture_desc.Height = texture.height();
  texture_desc.MipLevels = 1;
  texture_desc.ArraySize = 1;
  texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  texture_desc.SampleDesc.Count = 1;
  texture_desc.Usage = D3D11_USAGE_DEFAULT;
  texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  ID3D11Texture2D* tex;
  D3D11_SUBRESOURCE_DATA data;
  data.pSysMem = texture.pixels();
  data.SysMemPitch = texture.width() * sizeof resources::Texel;
  
  if (FAILED(d3d11_device_->CreateTexture2D(&texture_desc, &data, &tex)))
  {
    printf("failed to create texture for model");
    return false;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_desc{};
  shader_resource_desc.Format = texture_desc.Format;
  shader_resource_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shader_resource_desc.Texture2D.MostDetailedMip = 0;
  shader_resource_desc.Texture2D.MipLevels = 1;

  ID3D11ShaderResourceView* srv;
  if (FAILED(d3d11_device_->CreateShaderResourceView(tex, &shader_resource_desc, &srv)))
  {
    printf("failed to create Shader resource view");
    return false;
  }
  tex->Release();

  GPUTextureResourceHandle* handle = texture.gpu_handle();
  *handle = std::make_unique<GPUTextureResource>(srv);
  return true;
}

//-------------------------------------------------------------------------------------------------
DrawCall D3D11Renderer::DrawModel(DrawCall call, RenderTargets render_target)
{
  if ( !call.model )
  {
    return call;
  }
  cb_per_obj.world = call.world;
  GetViewProjectionMatrix(render_target, cb_per_obj.view, cb_per_obj.persp);
  cb_per_obj.invers_transpose_world = glm::transpose(glm::inverse(call.world));

  d3d11_device_context_->UpdateSubresource(cb_per_object_buffer_, 0, NULL, &cb_per_obj, 0, 0);
  d3d11_device_context_->VSSetConstantBuffers(0, 1, &cb_per_object_buffer_);

  GPUModelResourceHandle handle = *call.model->gpu_handle();
  uint32_t stride = sizeof(resources::Vertex);
  uint32_t offset = 0;
  d3d11_device_context_->IASetVertexBuffers(0, 1, &handle->vert_buffer_, &stride, &offset);
  d3d11_device_context_->IASetIndexBuffer(handle->idx_buffer_, DXGI_FORMAT_R16_UINT, 0);

  size_t cur_pos = 0;
  for (size_t i = 0; i < handle->vert_offsets_.size(); ++i)
  {
    cb_material_per_mesh mcb;
    resources::Material mat = call.model->meshes()[i].material();
    mcb.diffuse = mat.base_color_factor.GetNormalized();
    //mcb.emissive = mat.emissive_factor.GetNormalized();
   
     
    d3d11_device_context_->UpdateSubresource(cb_material_per_object_buffer_, 0, NULL, &mcb, 0, 0);
    d3d11_device_context_->PSSetConstantBuffers(0, 1, &cb_material_per_object_buffer_);
    GPUTextureResourceHandle tex = *mat.base_color_texture->gpu_handle();
    if (tex != nullptr)
    {
      d3d11_device_context_->PSSetShaderResources(0, 1, &(*mat.base_color_texture->gpu_handle())->texture_);
    }

    d3d11_device_context_->DrawIndexed((uint32_t)handle->num_idices_[i], (uint32_t)cur_pos, (int32_t)handle->vert_offsets_[i]);
    cur_pos += handle->num_idices_[i];
  }
  return call;
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::DrawDebug(RenderTargets render_target)
{
  d3d11_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  cb_per_obj.world = { 1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1 };
  GetViewProjectionMatrix(render_target, cb_per_obj.view, cb_per_obj.persp);

  d3d11_device_context_->UpdateSubresource(cb_per_object_buffer_, 0, NULL, &cb_per_obj, 0, 0);
  d3d11_device_context_->VSSetConstantBuffers(0, 1, &cb_per_object_buffer_);

  d3d11_device_context_->UpdateSubresource(line_buffer_, 0, NULL, line_vertices, 0, 0);
  uint32_t stride = sizeof(DebugVertex);
  uint32_t offset = 0;
  d3d11_device_context_->IASetVertexBuffers(0, 1, &line_buffer_, &stride, &offset);
  d3d11_device_context_->Draw(current_line_count_ * 2, 0);
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::GetViewProjectionMatrix(RenderTargets& target, Mat44& view, Mat44& projection)
{
  switch (target)
  {
  case RenderTargets::kLeftEye:
    view = vr_cam.view_left_;
    projection = glm::transpose(vr_cam.left_eye_projection_);
    break;
  case RenderTargets::kRightEye:
    view = vr_cam.view_right_;
    projection = glm::transpose(vr_cam.right_eye_projection_);
    break;
  case RenderTargets::kDebugCamera:
    view = glm::transpose(current_camera_->view());
    projection = glm::transpose(current_camera_->perspective());
    break;
  }
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::AddToDrawQueue(std::shared_ptr<resources::Model> handle, const Mat44& world)
{
   draw_queue_.push( { handle  , world } );
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::SetRenderState(RenderModes mode)
{
  switch (mode)
  {
  case RenderModes::kSolid:
    d3d11_device_context_->RSSetState(solid_);
    break;
  case RenderModes::kWireframe:
    d3d11_device_context_->RSSetState(wireframe_);
    break;
  }
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::RenderDrawQueue(Input* input)
{
  // if (vr_system_ != nullptr)
  // {
  //   d3d11_device_context_->RSSetViewports(1, &vr_viewport_);
  //   // setup the vr camera
  //   vr::HmdMatrix34_t pose = vr_system_->GetEyeToHeadTransform(vr::Eye_Left);
  //   vr_cam.left_eye_view_ = VrMat34ToMat44(pose);
  //   vr_cam.left_eye_view_ = glm::inverse(vr_cam.left_eye_view_);
  // 
  //   pose = vr_system_->GetEyeToHeadTransform(vr::Eye_Right);
  //   vr_cam.right_eye_view_ = VrMat34ToMat44(pose);
  //   vr_cam.right_eye_view_ = glm::inverse(vr_cam.right_eye_view_);
  // 
  //   vr::HmdMatrix44_t projection = vr_system_->GetProjectionMatrix(vr::Eye_Left, 1.0f, 10000.0f);
  //   vr_cam.left_eye_projection_ = VrMat44ToMat44(projection); 
  // 
  //   projection = vr_system_->GetProjectionMatrix(vr::Eye_Right, 1.0f, 10000.0f);
  //   vr_cam.right_eye_projection_ = VrMat44ToMat44(projection);
  // 
  //   vr_cam.view_left_ = glm::transpose(vr_cam.left_eye_view_ * hmd_pose);
  //   vr_cam.view_right_ = glm::transpose(vr_cam.right_eye_view_ * hmd_pose);
  // 
  //   // clear the left eye texture
  //   ID3D11RenderTargetView* render_target = left_eye_->render_target();
  //   d3d11_device_context_->OMSetRenderTargets(1, &render_target, vr_depth_stencil_view_);
  //   d3d11_device_context_->ClearRenderTargetView(render_target, clear_color_);
  //   d3d11_device_context_->ClearDepthStencilView(vr_depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  // 
  //   // set the shader to use the model shader
  //   d3d11_device_context_->VSSetShader(vs_, 0, 0);
  //   d3d11_device_context_->PSSetShader(ps_, 0, 0);
  //   d3d11_device_context_->IASetInputLayout(vert_layout_);
  // 
  //   // draw the scene to the left eye texture
  //   for (size_t i = 0; i < draw_queue_.size(); ++i)
  //   {
  //     DrawCall handle = draw_queue_.front();
  //     draw_queue_.pop();
  //     draw_queue_.push(DrawModel(handle, RenderTargets::kLeftEye));
  //   }
  //   d3d11_device_context_->VSSetShader(vs_debug_, 0, 0);
  //   d3d11_device_context_->PSSetShader(ps_debug_, 0, 0);
  //   d3d11_device_context_->IASetInputLayout(vert_layout_debug_);
  //   DrawDebug(RenderTargets::kLeftEye);
  // 
  //   // clear the right eye texture
  //   render_target = right_eye_->render_target();
  //   d3d11_device_context_->OMSetRenderTargets(1, &render_target, vr_depth_stencil_view_);
  //   d3d11_device_context_->ClearRenderTargetView(render_target, clear_color_);
  //   d3d11_device_context_->ClearDepthStencilView(vr_depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  // 
  //   // set the shader to use the model shgader
  //   d3d11_device_context_->VSSetShader(vs_, 0, 0);
  //   d3d11_device_context_->PSSetShader(ps_, 0, 0);
  //   d3d11_device_context_->IASetInputLayout(vert_layout_);
  // 
  //   // draw the scene to the right eye texture
  //   // Likely a different container needs to be used.
  //   for (size_t i = 0; i < draw_queue_.size(); ++i)
  //   {
  //     DrawCall handle = draw_queue_.front();
  //     draw_queue_.pop();
  //     draw_queue_.push(DrawModel(handle, RenderTargets::kRightEye));
  //   }
  //   d3d11_device_context_->VSSetShader(vs_debug_, 0, 0);
  //   d3d11_device_context_->PSSetShader(ps_debug_, 0, 0);
  //   d3d11_device_context_->IASetInputLayout(vert_layout_debug_);
  //   DrawDebug(RenderTargets::kRightEye);
  // }

  // d3d11_device_context_->UpdateSubresource()
  
  d3d11_device_context_->RSSetViewports(1, &debug_viewport_);
  d3d11_device_context_->OMSetRenderTargets(1, &debug_render_target_view_, debug_depth_stencil_view_);
  d3d11_device_context_->ClearRenderTargetView(debug_render_target_view_, clear_color_);
  d3d11_device_context_->ClearDepthStencilView(debug_depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  // set the shader to use the model shgader
  d3d11_device_context_->VSSetShader(vs_, 0, 0);
  d3d11_device_context_->PSSetShader(ps_, 0, 0);
  d3d11_device_context_->PSSetSamplers(0, 1, &default_sampler_);



  // update the light constant buffer
  cb_lights_per_frame lcb;
  lcb.eye_position = Vec4(current_camera_->transform.position(), 1.0f);
  lcb.global_ambient = Vec4(1,1,1,1);
  if (lights_.size() > MAX_LIGHTS)
  {
    printf("light buffer exceeds maximum capacity. only 8 lights are allowed, %llu were added", lights_.size());
    memcpy_s(lcb.lights, sizeof(Light) * MAX_LIGHTS, lights_.data(), sizeof(Light) * MAX_LIGHTS);
  }
  else
  {
    memcpy_s(lcb.lights, sizeof(Light) * lights_.size(), lights_.data(), sizeof(Light) *lights_.size());
    lcb.num_lights = (int)lights_.size();
  }
  lights_.clear();

  d3d11_device_context_->UpdateSubresource(cb_lights_per_draw_buffer_, 0, NULL, &lcb, 0, 0);
  d3d11_device_context_->PSSetConstantBuffers(1, 1, &cb_lights_per_draw_buffer_);

  d3d11_device_context_->IASetInputLayout(vert_layout_);

  // draw the scene a third time with a free cam so we can debug along side the scene
  for (; draw_queue_.size() > 0; draw_queue_.pop())
  {
    DrawModel(draw_queue_.front(), RenderTargets::kDebugCamera);
  }


  d3d11_device_context_->VSSetShader(vs_debug_, 0, 0);
  d3d11_device_context_->PSSetShader(ps_debug_, 0, 0);
  d3d11_device_context_->IASetInputLayout(vert_layout_debug_);
  DrawDebug(RenderTargets::kDebugCamera);
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::ReadShader(const char* shader_name, std::string& buffer)
{
  std::string file_path = "./shaders/";
  file_path.append(shader_name);
  file_path.append(".cso");
  std::ifstream file(file_path, std::ios::binary);
  
  if (file.good() == true)
  {
    buffer = std::string((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
  }
  else
  {
    printf("unable to load shader %s", file_path.c_str());
  }
}

//-------------------------------------------------------------------------------------------------
void D3D11Renderer::UpdatateHmdPose()
{
  if (!vr_system_)
    return;

  vr::VRCompositor()->WaitGetPoses(tracked_device_pose_, vr::k_unMaxTrackedDeviceCount, NULL, 0);

  for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
  {
    if (tracked_device_pose_[nDevice].bPoseIsValid)
    {
      device_pose[nDevice] = VrMat34ToMat44(tracked_device_pose_[nDevice].mDeviceToAbsoluteTracking);
    }
  }

  if (tracked_device_pose_[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
  {
    hmd_pose = glm::inverse(device_pose[vr::k_unTrackedDeviceIndex_Hmd]);
  }
  else
  {
    printf("pose not valid");
  }
}
