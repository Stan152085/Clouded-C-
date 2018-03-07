#pragma once
#include "Material.h"
#include "Vertex.h"

#include <vector>

namespace resources
{
  class Mesh
  {
  public:
    Mesh(
      const std::vector<unsigned short>& indices,
      const std::vector<Vertex>& vertices,
      const Material& material);

    const std::vector<unsigned short>& indices() const
    {
      return indices_;
    }
    const std::vector<Vertex>& vertices() const
    {
      return vertices_;
    }

  private:
    std::vector<unsigned short> indices_;
    std::vector<Vertex> vertices_;
    Material material_;
  };
}