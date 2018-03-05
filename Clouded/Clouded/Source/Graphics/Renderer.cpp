#include "stdafx.h"
#include "renderer.h"
#include "fstream"
#include "../Resources/Model.h"
#include <D3D11.h>

// hardcoded triangle for now
resources::Vertex triangle[] = {
  resources::Vertex{ Vec3{ 0.0f, 0.5f, 0.5f } },
  resources::Vertex{ Vec3{ 0.5f, -0.5f, 0.5f } },
  resources::Vertex{ Vec3{ -0.5f, -0.5f, 0.5f } }
};

D3D11Renderer::D3D11Renderer() :
  clear_color_{ 1.0f ,1.0f ,1.0f ,1.0f }
{
}

D3D11Renderer::~D3D11Renderer()
{
}

bool D3D11Renderer::Intialize(HWND window_handle, const Vec2u& screen_size)
{
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
                                NULL,
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
    printf("Failed to create swap chain");
  }

  ID3D11Texture2D* back_buffer;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
  d3d11_device_->CreateRenderTargetView(back_buffer, NULL, &render_target_view_);
  back_buffer->Release();
  d3d11_device_context_->OMSetRenderTargets(1, &render_target_view_, NULL);

  /*set up shaders and vertex buffers to render a triangle*/
  std::vector<char> vs_buf;
  std::vector<char> ps_buf;
  ReadShader("VertexShader", vs_buf);
  d3d11_device_->CreateVertexShader(vs_buf.data(), vs_buf.size(), NULL, &vs_);

  ReadShader("PixelShader", ps_buf);
  d3d11_device_->CreatePixelShader(ps_buf.data(), ps_buf.size(), NULL, &ps_);

  d3d11_device_context_->VSSetShader(vs_, 0, 0);
  d3d11_device_context_->PSSetShader(ps_, 0, 0);

  D3D11_BUFFER_DESC vs_buffer_desc;
  ZeroMemory(&vs_buffer_desc, sizeof(D3D11_BUFFER_DESC));

  vs_buffer_desc.ByteWidth = sizeof(resources::Vertex) * 3;
  vs_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vs_buffer_desc.CPUAccessFlags = NULL;
  vs_buffer_desc.MiscFlags = NULL;
  vs_buffer_desc.StructureByteStride = NULL;
  vs_buffer_desc.Usage = D3D11_USAGE_DEFAULT;

  D3D11_SUBRESOURCE_DATA buffer_data;
  ZeroMemory(&buffer_data, sizeof(D3D11_SUBRESOURCE_DATA));

  buffer_data.pSysMem = triangle;
  d3d11_device_->CreateBuffer(&vs_buffer_desc, &buffer_data, &triangle_vert_buffer_);

  uint32_t stride = sizeof(resources::Vertex);
  d3d11_device_context_->IASetVertexBuffers(0, 1, &triangle_vert_buffer_, &stride, 0);
  d3d11_device_->CreateInputLayout(layout, 1, vs_buf.data(), vs_buf.size(), &vert_layout_);
  d3d11_device_context_->IASetInputLayout(vert_layout_);
  d3d11_device_context_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D11_VIEWPORT viewport;
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.Width = screen_size.x;
  viewport.Height = screen_size.y;
  d3d11_device_context_->RSSetViewports(1, &viewport);


  return true;
}

bool D3D11Renderer::Release()
{
  swap_chain_->Release();
  d3d11_device_->Release();
  d3d11_device_context_->Release();
  render_target_view_->Release();
  triangle_vert_buffer_->Release();
  vs_->Release();
  ps_->Release();
  vs_buffer_->Release();
  ps_buffer_->Release();
  vert_layout_->Release();
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

void D3D11Renderer::Draw()
{
  d3d11_device_context_->ClearRenderTargetView(render_target_view_, clear_color_);
  d3d11_device_context_->Draw(3, 0);
  swap_chain_->Present(0, 0);
}

void D3D11Renderer::ReadShader(const char* shader_name, std::vector<char> buffer)
{
  std::string file_path = "./Shaders/";
  file_path.append(shader_name);
  file_path.append(".cso");
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);
  if (file.good() == true)
  {
    size_t size = file.tellg();
    buffer.reserve(size);
    file.seekg(std::ios::beg);
    file.read(buffer.data(), size);
  }
  else
  {
    printf("unable to load shader %s", file_path.c_str());
  }
}
