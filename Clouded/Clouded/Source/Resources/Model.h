#pragma once
#include "Mesh.h"

namespace resources
{
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