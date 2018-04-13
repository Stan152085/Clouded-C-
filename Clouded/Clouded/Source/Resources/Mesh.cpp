#include "stdafx.h"
#include "Mesh.h"

resources::Mesh::Mesh(
  const std::vector<unsigned short>& indices,
  const std::vector<Vertex>& vertices,
  const Material& material,
  const Transform& l2w)
  :
  indices_(indices),
  vertices_(vertices),
  material_(material),
  l2w_(l2w)
{
}
