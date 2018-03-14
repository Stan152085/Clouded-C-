#include "stdafx.h"
#include "Mesh.h"

resources::Mesh::Mesh(
  const std::vector<unsigned short>& indices,
  const std::vector<Vertex>& vertices,
  const Material& material)
  :
  indices_(indices),
  vertices_(vertices),
  material_(material)
{
}
