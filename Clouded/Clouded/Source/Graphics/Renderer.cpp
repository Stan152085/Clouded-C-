#include "stdafx.h"
#include "renderer.h"
#include "fstream"
#include "Core\Transform.h"
#include "Resources\Model.h"
#include <D3D11.h>
#include <vector>
#include <glm\gtc\matrix_transform.hpp>

const unsigned int max_line_count_ = 500;

struct Camera
{
  Camera() = default;

  Camera(float width, float height)
  {
    view = glm::lookAtLH(pos, target, up); // left handed winding order
    persp = glm::perspectiveFovLH_ZO((float)glm::radians(90.0f), width, height, 1.0f, 1000.0f);
  }
  void Move(float value)
  {
    pos.z -= value;
    view = glm::lookAtLH(pos, target, up); // left handed winding order
  }
  Vec3 pos = Vec3(0.0f, 3.0f, -8.0f);
  Vec3 target = Vec3(0.0f, 0.0f, 0.0f);
  Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

  Mat44 view;
  Mat44 persp;

};

std::vector<resources::G_Vertex> line_vertices;
struct constant_buffer
{
  Mat44 world;
  Mat44 view;
  Mat44 persp;
};

constant_buffer cb_per_obj;
Camera cam;

Transform obj0;
Transform obj1;

float rot = 0.01f;

// hardcoded quad for now
resources::G_Vertex quad_vertices[] = {
  resources::G_Vertex{ Vec3{ -0.5f, -0.5f, 1.7f }, Vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  resources::G_Vertex{ Vec3{ -0.5f, 0.5f,  1.7f }, Vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  resources::G_Vertex{ Vec3{ 0.5f, 0.5f,   1.7f }, Vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  resources::G_Vertex{ Vec3{ 0.5f, -0.5f,  1.7f }, Vec4(0.0f, 1.0f, 0.0f, 1.0f) }
};

uint32_t quad_indices[] = {
  0, 1, 2,
  0, 2, 3
};

// hardcoded cube
resources::G_Vertex cube_vertices[] = {
  resources::G_Vertex{ Vec3{ -1.0f, -1.0f, -1.0f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } },
  resources::G_Vertex{ Vec3{ -1.0f,  1.0f, -1.0f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f } },
  resources::G_Vertex{ Vec3{  1.0f,  1.0f, -1.0f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f } },
  resources::G_Vertex{ Vec3{  1.0f, -1.0f, -1.0f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } },
  resources::G_Vertex{ Vec3{ -1.0f, -1.0f,  1.0f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f } },
  resources::G_Vertex{ Vec3{ -1.0f,  1.0f,  1.0f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f } },
  resources::G_Vertex{ Vec3{  1.0f,  1.0f,  1.0f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } },
  resources::G_Vertex{ Vec3{  1.0f, -1.0f,  1.0f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f } }
};

uint32_t cube_indices[] = {
  // front face
  0, 1, 2,
  0, 2, 3,

  // back face
  4, 6, 5,
  4, 7, 6,

  // left face
  4, 5, 1,
  4, 1, 0,

  // right face
  3, 2, 6,
  3, 6, 7,

  // top face
  1, 5, 6,
  1, 6, 2,

  // bottom face
  4, 0, 3,
  4, 3, 7
};

D3D11Renderer::D3D11Renderer() :
  clear_color_{ 0.0f ,0.0f ,0.0f ,0.0f }
{
}

D3D11Renderer::~D3D11Renderer()
{
}

bool D3D11Renderer::Intialize(HWND window_handle, const Vec2u& screen_size)
{
  cam = Camera((float)screen_size.x, (float)screen_size.y);

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
  }

  ID3D11Texture2D* back_buffer;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
  d3d11_device_->CreateRenderTargetView(back_buffer, NULL, &render_target_view_);
  back_buffer->Release();

  D3D11_TEXTURE2D_DESC depth_stencil_desc;
  // ZeroMemory(&depth_stencil_desc, sizeof(D3D11_TEXTURE2D_DESC));
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
  d3d11_device_->CreateTexture2D(&depth_stencil_desc, NULL, &depth_stencil_buffer_);
  d3d11_device_->CreateDepthStencilView(depth_stencil_buffer_, NULL, &depth_stencil_view_);

  d3d11_device_context_->OMSetRenderTargets(1, &render_target_view_, depth_stencil_view_);
  
  /*set up shaders and vertex buffers to render a triangle*/
  std::vector<char> vs_buf;
  std::vector<char> ps_buf;
  ReadShader("VertexShader", vs_buf);
  result = d3d11_device_->CreateVertexShader(vs_buf.data(), vs_buf.size(), NULL, &vs_);

  if (FAILED(result))
  {
    printf("Failed to create vertex shader \n");
  }

  ReadShader("PixelShader", ps_buf);
  result = d3d11_device_->CreatePixelShader(ps_buf.data(), ps_buf.size(), NULL, &ps_);

  if (FAILED(result))
  {
    printf("Failed to create pixel shader \n");
  }

  d3d11_device_context_->VSSetShader(vs_, 0, 0);
  d3d11_device_context_->PSSetShader(ps_, 0, 0);

  // create index and vertex buffer for a primitive or model
  // D3D11_BUFFER_DESC idx_buffer_desc;
  // ZeroMemory(&idx_buffer_desc, sizeof(D3D11_BUFFER_DESC));
  // idx_buffer_desc.ByteWidth = sizeof(cube_indices);
  // idx_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  // idx_buffer_desc.CPUAccessFlags = NULL;
  // idx_buffer_desc.MiscFlags = NULL;
  // idx_buffer_desc.StructureByteStride = NULL;
  // idx_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  // 
  // D3D11_SUBRESOURCE_DATA idx_buffer_data;
  // ZeroMemory(&idx_buffer_data, sizeof(D3D11_SUBRESOURCE_DATA));
  // idx_buffer_data.pSysMem = cube_indices;
  // d3d11_device_->CreateBuffer(&idx_buffer_desc, &idx_buffer_data, &index_buffer_);
  // d3d11_device_context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
  // 
  // D3D11_BUFFER_DESC vs_buffer_desc;
  // ZeroMemory(&vs_buffer_desc, sizeof(D3D11_BUFFER_DESC));
  // vs_buffer_desc.ByteWidth = sizeof(cube_vertices);
  // vs_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  // vs_buffer_desc.CPUAccessFlags = NULL;
  // vs_buffer_desc.MiscFlags = NULL;
  // vs_buffer_desc.StructureByteStride = NULL;
  // vs_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  // 
  // D3D11_SUBRESOURCE_DATA vs_buffer_data;
  // ZeroMemory(&vs_buffer_data, sizeof(D3D11_SUBRESOURCE_DATA));
  // vs_buffer_data.pSysMem = cube_vertices;
  // d3d11_device_->CreateBuffer(&vs_buffer_desc, &vs_buffer_data, &vert_buffers_[0]);
  
  /*create line buffer*/
  D3D11_BUFFER_DESC line_buffer_desc;
  ZeroMemory(&line_buffer_desc, sizeof(D3D11_BUFFER_DESC));
  line_buffer_desc.ByteWidth = max_line_count_ * 2;
  line_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  line_buffer_desc.CPUAccessFlags = NULL;
  line_buffer_desc.MiscFlags = NULL;
  line_buffer_desc.StructureByteStride = NULL;
  line_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

  result = d3d11_device_->CreateBuffer(&line_buffer_desc, NULL, &vert_buffers_[1]);

  uint32_t stride = sizeof(resources::G_Vertex);
  uint32_t offset = 0;
  d3d11_device_context_->IASetVertexBuffers(0, 1, &line_buffer_, &stride, &offset);
  // create input layout and set primitive topology
  d3d11_device_->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vs_buf.data(), vs_buf.size(), &vert_layout_);
  d3d11_device_context_->IASetInputLayout(vert_layout_);
  d3d11_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  assert(vert_layout_ != nullptr);

  D3D11_VIEWPORT viewport;
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.Width = (float)screen_size.x;
  viewport.Height = (float)screen_size.y;
  d3d11_device_context_->RSSetViewports(1, &viewport);

  // create constant buffer per object
  D3D11_BUFFER_DESC cb_per_obj_desc;
  ZeroMemory(&cb_per_obj_desc, sizeof(D3D11_BUFFER_DESC));
  cb_per_obj_desc.ByteWidth = sizeof(constant_buffer);
  cb_per_obj_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cb_per_obj_desc.CPUAccessFlags = NULL;
  cb_per_obj_desc.MiscFlags = NULL;
  cb_per_obj_desc.Usage = D3D11_USAGE_DEFAULT;

  result = d3d11_device_->CreateBuffer(&cb_per_obj_desc, NULL, &cb_per_object_buffer_);

  D3D11_RASTERIZER_DESC wireframe_desc;
  ZeroMemory(&wireframe_desc, sizeof(D3D11_RASTERIZER_DESC));
  wireframe_desc.FillMode = D3D11_FILL_WIREFRAME;
  wireframe_desc.CullMode = D3D11_CULL_NONE;
  result = d3d11_device_->CreateRasterizerState(&wireframe_desc, &wireframe_);
  d3d11_device_context_->RSSetState(wireframe_);
  return true;
}

bool D3D11Renderer::Release()
{
  swap_chain_->Release();
  d3d11_device_->Release();
  d3d11_device_context_->Release();
  render_target_view_->Release();
  line_buffer_->Release();
  index_buffer_->Release();
  depth_stencil_buffer_->Release();
  depth_stencil_view_->Release();
  vs_->Release();
  ps_->Release();
  vs_buffer_->Release();
  ps_buffer_->Release();
  vert_layout_->Release();
  cb_per_object_buffer_->Release();
  wireframe_->Release();
  return true;
}

void D3D11Renderer::SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  float mult = 1 / 255;
  clear_color_[0] = { mult * r };
  clear_color_[1] = { mult * g };
  clear_color_[2] = { mult * b };
  clear_color_[3] = { mult * a };
}

void D3D11Renderer::DrawLine(const Vec3 & start, const Vec3 end)
{
  line_vertices.push_back({ start, Vec4(0.0f,1.0f,0.0f,1.0f) });
  line_vertices.push_back({ end, Vec4(0.0f,1.0f,0.0f,1.0f) });
}

void D3D11Renderer::Draw()
{
  DrawLine(Vec3(-2.0f, 2.0f, 0.0f), Vec3(2.0f, -2.0f, 0.0f));
  DrawLine(Vec3(2.0f, -2.0f, 0.0f), Vec3(-2.0f, -2.0f, 0.0f));

  /*temp scene update*/
  rot += 0.0005f;
  if (rot > 6.28f)
  {
    rot = 0.0f;
  }

  Mat44 obj0_world;
  Mat44 rotation = glm::rotate(rot, Vec3(0, 1, 0));
  Mat44 translation = glm::translate(Vec3(0, 0, 0));
  obj0_world = translation * rotation;

  Mat44 obj1_local;
  rotation = glm::rotate(rot, Vec3(0,1,0));
  Mat44 scale = glm::scale(Vec3(1.3f, 1.3f, 1.3f));
  translation = glm::translate(Vec3(0, 0, 4));
  obj1_local = translation * rotation * scale;

  Mat44 obj1_world = obj0_world * obj1_local;

  d3d11_device_context_->ClearRenderTargetView(render_target_view_, clear_color_);
  d3d11_device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  /*draw obj0*/
  cb_per_obj.world = glm::transpose(obj0_world);
  cb_per_obj.view = glm::transpose(cam.view);
  cb_per_obj.persp = glm::transpose(cam.persp);
  d3d11_device_context_->UpdateSubresource(cb_per_object_buffer_, 0, NULL, &cb_per_obj, 0, 0);
  d3d11_device_context_->VSSetConstantBuffers(0, 1, &cb_per_object_buffer_);
  d3d11_device_context_->DrawIndexed(36, 0, 0);

  /*draw obj1*/
  cb_per_obj.world = glm::transpose(obj1_world);
  d3d11_device_context_->UpdateSubresource(cb_per_object_buffer_, 0, NULL, &cb_per_obj, 0, 0);
  d3d11_device_context_->VSSetConstantBuffers(0, 1, &cb_per_object_buffer_);
  d3d11_device_context_->DrawIndexed(36, 0, 0);

  cb_per_obj.world = { 1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1 };
  d3d11_device_context_->UpdateSubresource(cb_per_object_buffer_, 0, NULL, &cb_per_obj, 0, 0);
  d3d11_device_context_->UpdateSubresource(vert_buffers_[1], 0, NULL, line_vertices.data(), 0, 0);

  uint32_t stride = sizeof(resources::G_Vertex);
  uint32_t offset = 0;
  d3d11_device_context_->IASetVertexBuffers(0, 1, &vert_buffers_[1], &stride, &offset);
  d3d11_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  d3d11_device_context_->Draw((unsigned int)line_vertices.size(), 0);
  line_vertices.clear();

  swap_chain_->Present(0, 0);

  stride = sizeof(resources::G_Vertex);
  offset = 0;
  d3d11_device_context_->IASetVertexBuffers(0, 1, &vert_buffers_[0], &stride, &offset);
  d3d11_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Renderer::ReadShader(const char* shader_name, std::vector<char>& buffer)
{
  std::string file_path = "./shaders/";
  file_path.append(shader_name);
  file_path.append(".cso");
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);
  if (file.good() == true)
  {
    size_t size = file.tellg();
    buffer.resize(size);
    file.seekg(std::ios::beg);
    file.read(buffer.data(), size);
  }
  else
  {
    printf("unable to load shader %s", file_path.c_str());
  }
}
