#include "stdafx.h"
#include "Model.h"
#include "Mesh.h"

void resources::Model::AddMesh(const Mesh & mesh)
{
  meshes_.push_back(mesh);
}
