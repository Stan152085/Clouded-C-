#pragma once
#include <vector>
#include <memory>
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct Material;

struct GPUModelResource
{
  GPUModelResource( ID3D11Buffer* const vert_buffer, ID3D11Buffer* const idx_buffer, const std::vector<size_t> num_idices, const std::vector<size_t> vert_offsets) :
    vert_buffer_( vert_buffer ),
    idx_buffer_( idx_buffer ),
    num_idices_( num_idices ),
    vert_offsets_( vert_offsets )
  {

  }

  ID3D11Buffer* const vert_buffer_;
  ID3D11Buffer* const idx_buffer_;
  const std::vector<size_t> num_idices_; //number of indices per mesh
  const std::vector<size_t> vert_offsets_; //offset of the first vertex for each mesh
};

using GPUModelResourceHandle = std::shared_ptr<GPUModelResource>;