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
    const std::vector<Mesh>& meshes() const
    {
      return meshes_;
    }

  private:
    std::vector<Mesh> meshes_;
  };
}
