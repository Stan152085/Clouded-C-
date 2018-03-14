#pragma once
#include "Mesh.h"

namespace resources
{
  struct G_Vertex
  {
    Vec3 position;
    Vec4 color;
  };

  class Model
  {
  public:
    const std::vector<Mesh>& meshes() const
    {
      return meshes_;
    }

    void AddMesh(const Mesh& mesh);

  private:
    std::vector<Mesh> meshes_;
  };
}
