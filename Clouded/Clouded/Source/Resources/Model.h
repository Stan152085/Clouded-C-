#pragma once

struct GPUModelResource;
using GPUModelResourceHandle = std::shared_ptr<GPUModelResource>;


namespace resources
{
  class Mesh;
  class Model
  {
  public:
    const std::vector<Mesh>& meshes() const
    {
      return meshes_;
    }

    void AddMesh(const Mesh& mesh);

    GPUModelResourceHandle* gpu_handle()
    {
      return &gpu_handle_;
    }
  private:
    std::vector<Mesh> meshes_;
    GPUModelResourceHandle gpu_handle_;
  };
}
