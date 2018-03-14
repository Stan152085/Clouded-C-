#pragma once
#include "Mesh.h"

namespace resources
{
  struct Vertex
  {
    Vec3 position;
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
