#include "stdafx.h"
#include "AssetManager.h"
#include "Model.h"
#include "Texture.h"
#include "ResourceManagementUtils.h"
#include "Core/Transform.h"

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef _DEBUG 
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#endif

#include "Graphics/renderer.h"

#include "TinyGLTF/tiny_gltf.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>

namespace resources
{
  // private load function, to wrap tinygltf
  bool Load(std::string & file, tinygltf::Model& result, std::string& err)
  {
    tinygltf::TinyGLTF gltf_ctx;
    std::string ext = GetFilePathExtension(file);

    bool ret = false;
    if (ext.compare("glb") == 0) {
      //std::cout << "Reading binary glTF" << std::endl;
      // assume binary glTF.
      ret = gltf_ctx.LoadBinaryFromFile(&result, &err, file.c_str());
    }
    else {
      //std::cout << "Reading ASCII glTF" << std::endl;
      // assume ascii glTF.
      ret = gltf_ctx.LoadASCIIFromFile(&result, &err, file.c_str());
    }

    if (!err.empty()) {
      printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
      printf("Failed to parse glTF\n");
      return false;
    }

    return true;
  }

  template <typename T>
  void GetBufferData(
    const tinygltf::Model& source,
    const tinygltf::Accessor& acc,
    std::vector<T>& res)
  {
    const tinygltf::BufferView& buff_view = source.bufferViews[acc.bufferView];
    const tinygltf::Buffer& buff = source.buffers[buff_view.buffer];

    size_t stride = buff_view.byteStride;
    if (stride == 0)
    {
      stride = sizeof T;
    }

    size_t num_elements = (buff_view.byteLength - acc.byteOffset) /
      stride;
    res.reserve(num_elements);

    const unsigned char* data = buff.data.data() + buff_view.byteOffset + acc.byteOffset;
    for (size_t i = 0; i < acc.count; ++i, data += stride)
    {
      res.push_back(*(reinterpret_cast<const T*>(data)));
    }
  }

  // Get a model from the cache. If it doesn't exist yet, load it
  std::shared_ptr<Model> AssetManager::GetModel(std::string& file,
  std::string& err)
  {
    // try to find the model in the map
    auto it = model_map.find(file);
    if (it != model_map.end())
    {
      return it->second;
    }

    // if it doesn't exist, load it
    tinygltf::Model gltf_model;
    if (Load(file, gltf_model, err) == false)
    {
      // return a nullptr if load failed
      return nullptr;
    }

    // construct the clouded model from the gltf model
    std::shared_ptr<Model> res = std::make_shared<Model>();
    ConstructModel(gltf_model, *res, file);
    renderer_.PushToGPU(*res);

    // insert the new model into the map
    model_map.insert(std::make_pair(file, res));
    return res;
  }

  // Get a texture from the cache. If it doesn't exist yet, load it
  std::shared_ptr<resources::Texture> AssetManager::GetTexture(std::string& file,
    std::string& err)
  {
    // try to find the texture in the map
    auto it = texture_map.find(file);
    if (it != texture_map.end())
    {
      return it->second;
    }

    // if it doesn't exist, load it
    int width, height, components;
    unsigned char* data;
    data = stbi_load(file.data(), &width, &height, &components, 0);
    if (data == nullptr)
    {
      // return a nullptr if load failed
      return std::shared_ptr<resources::Texture>(nullptr);
    }

    // construct the clouded texture from the stb texture
    std::shared_ptr<Texture> res(new Texture(width, height, components, data));
    renderer_.PushToGPU(*res);
    // insert the new texture into the map
    texture_map.insert(std::make_pair(file, res));
    return res;
  }

  void AssetManager::CleanUp()
  {
    ModelMap::iterator model_it;
    for (model_it = model_map.begin(); model_it != model_map.end(); ++model_it)
    {
      if (model_it->second.use_count() < 2)
      {
        renderer_.ReleaseFromGPU(*model_it->second->gpu_handle());
        model_map.erase(model_it);
      }
    }
    model_map.clear();
    TextureMap::iterator texture_it;
    for (texture_it = texture_map.begin(); texture_it != texture_map.end(); ++texture_it)
    {
      if (texture_it->second.use_count() < 2)
      {
        //renderer.ReleaseFromGPU(texture_it->second);
        texture_map.erase(texture_it);
      }
    }
    texture_map.clear();
  }

  Transform GetMatFromNodeMat(const tinygltf::Node& node)
  {
    Transform res;
    if (node.matrix.size() == 16) // the node is valid
    {
      float mat[16];
      for (int i = 0; i < 16; ++i)
      {
        mat[i] = static_cast<float>(node.matrix[i]);
      }
      glm::mat4 transformation = *(glm::mat4*)mat; // your transformation matrix.
      glm::vec3 scale;
      glm::quat rot;
      glm::vec3 pos;
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::decompose(transformation, scale, rot, pos, skew, perspective);
      rot = glm::conjugate(rot);

      res.SetPosition(pos);
      res.SetScale(scale);
      res.SetRotation(rot);
    }
    return res;
  }
  Transform GetMatFromNodeTRS(const tinygltf::Node& node)
  {
    Transform res;
    if (node.translation.size() != 0)
    {
      res.SetPosition(*(Vec3*)node.translation.data());
    }
    if (node.scale.size() != 0)
    {
      res.SetScale(*(Vec3*)node.scale.data());
    }
    if (node.rotation.size() != 0)
    {
      res.SetRotation(*(Quat*)node.rotation.data());
    }
    return res;
  }

  void AssetManager::RecursiveNodeExtract(const tinygltf::Model& source, const tinygltf::Node& node,
    const std::vector<std::shared_ptr<Texture>>& textures, Model& result, Transform& l2w)
  {
    l2w = GetMatFromNodeMat(node) * GetMatFromNodeTRS(node);
    if (node.mesh >= 0)
    {
      for (int i = 0; i < source.meshes[node.mesh].primitives.size(); ++i)
      {
        ConstructMesh(source, source.meshes[node.mesh].primitives[i], textures, result, l2w);
      }
    }
    for (int i = 0; i < node.children.size(); ++i)
    {
      RecursiveNodeExtract(source, source.nodes[node.children[i]], textures, result, l2w);
    }
  }

  void AssetManager::ConstructModel(const tinygltf::Model& source, Model& result, const std::string& model_name)
  {
    // convert and store all textures
    std::vector<std::shared_ptr<Texture>> textures;
    {
      for (int i = 0; i < source.textures.size(); ++i)
      {
        const tinygltf::Image& image = source.images[source.textures[i].source];
        auto it = texture_map.find(image.uri);
        if (it == texture_map.end())
        {
          // construct a new Texture from the gltf texture
          Texture* tex = new Texture(image.width, image.height, image.component, image.image.data());
          std::shared_ptr<Texture> shared_tex(tex);

          // store the image in the texture map
          renderer_.PushToGPU(*shared_tex);
          texture_map.insert(std::make_pair(image.uri, shared_tex));

          // store the texture in a local vector for easy referencing
         
          textures.push_back(shared_tex);
        }
        else
        {
          // store the found texture (unduplicated) in a local vector for easy referencing
          textures.push_back(it->second);
        }
      }
    }

    // convert all meshes and store them in the model
    // in gltf, our models are their meshes and our meshes are their primitives
    Transform l2w;
    for (int i = 0; i < source.scenes.size(); ++i)
    {
      for (int j = 0; j < source.scenes[i].nodes.size(); ++j)
      {
        RecursiveNodeExtract(source, source.nodes[source.scenes[i].nodes[j]],textures, result, l2w);
      }
    }
  }

  void AssetManager::ConstructMesh(
    const tinygltf::Model & source,
    const tinygltf::Primitive& prim,
    const std::vector<std::shared_ptr<Texture>>& textures,
    Model& result, const Transform& l2w)
  {
    ////////////////////
    // material
    ////////////////////
    Material mat;
    {
      const tinygltf::Material& gltf_mat = source.materials[prim.material];

      // name
      {
        auto it = gltf_mat.additionalValues.find("name");
        if (it != gltf_mat.additionalValues.end())
        {
          mat.name = it->second.string_value;
        }
        else
        {
          mat.name = "";
        }
      }

      // base color
      {
        auto it = gltf_mat.values.find("baseColorFactor");
        if (it != gltf_mat.values.end())
        {
          const std::vector<double>& arr = it->second.number_array;
          mat.base_color_factor = Texel(
            static_cast<unsigned char>(arr[0] * 255.0),
            static_cast<unsigned char>(arr[1] * 255.0),
            static_cast<unsigned char>(arr[2] * 255.0),
            static_cast<unsigned char>(arr[3] * 255.0));
        }
        else
        {
          mat.base_color_factor = Texel(255);
        }

        it = gltf_mat.values.find("baseColorTexture");
        if (it != gltf_mat.values.end())
        {
          mat.base_color_texture = textures[it->second.TextureIndex()];
        }
        else
        {
          mat.base_color_texture = std::shared_ptr<Texture>(nullptr);
        }
      }

      // normal
      {
        auto it = gltf_mat.values.find("normalTexture");
        if (it != gltf_mat.values.end())
        {
          mat.normal_texture = textures[it->second.TextureIndex()];

          auto it2 = it->second.json_double_value.find("scale");
          if (it2 != it->second.json_double_value.end())
          {
            mat.normal_scale = static_cast<float>(it2->second);
          }
          else
          {
            mat.normal_scale = 1.0f;
          }
        }
        else
        {
          mat.normal_texture = std::shared_ptr<Texture>(nullptr);
          mat.normal_scale = 1.0f;
        }
      }

      // occlusion
      {
        auto it = gltf_mat.values.find("occlusionTexture");
        if (it != gltf_mat.values.end())
        {
          mat.occlusion_texture = textures[it->second.TextureIndex()];

          auto it2 = it->second.json_double_value.find("strength");
          if (it2 != it->second.json_double_value.end())
          {
            mat.occlusion_strength = static_cast<float>(it2->second);
          }
          else
          {
            mat.occlusion_strength = 1.0f;
          }
        }
        else
        {
          mat.occlusion_texture = std::shared_ptr<Texture>(nullptr);
          mat.occlusion_strength = 1.0f;
        }
      }

      // emission
      {
        auto it = gltf_mat.values.find("emissiveTexture");
        if (it != gltf_mat.values.end())
        {
          mat.emissive_texture = textures[it->second.TextureIndex()];

          auto it2 = gltf_mat.values.find("emissiveFactor");
          if (it2 != gltf_mat.values.end())
          {
            const std::vector<double>& arr = it2->second.number_array;
            mat.base_color_factor = Texel(
              static_cast<unsigned char>(arr[0] * 255.0),
              static_cast<unsigned char>(arr[1] * 255.0),
              static_cast<unsigned char>(arr[2] * 255.0));
          }
          else
          {
            mat.emissive_factor = Texel(255);
          }
        }
        else
        {
          mat.emissive_texture = std::shared_ptr<Texture>(nullptr);
          mat.emissive_factor = Texel(255);
        }
      }
    }

    ////////////////////
    // indices
    ////////////////////
    std::vector<unsigned short> indices;
    {
      const tinygltf::Accessor& acc = source.accessors[prim.indices];
      GetBufferData<unsigned short>(source, acc, indices);
    }

    ////////////////////
    // vertices
    ////////////////////
    std::vector<Vertex> vertices;
    {
      // positions
      std::vector<Vec3> positions;
      {
        std::string name = std::string("POSITION");
        auto it = prim.attributes.find(name);
        if (it != prim.attributes.end())
        {
          const tinygltf::Accessor& acc = source.accessors[it->second];
          GetBufferData<Vec3>(source, acc, positions);
        }
      }

      // normals
      std::vector<Vec3> normals;
      {
        std::string name = std::string("NORMAL");
        auto it = prim.attributes.find(name);
        if (it != prim.attributes.end())
        {
          const tinygltf::Accessor& acc = source.accessors[it->second];
          GetBufferData<Vec3>(source, acc, normals);
        }
      }

      // tangents
      std::vector<Vec4> tangents;
      {
        std::string name = std::string("TANGENT");
        auto it = prim.attributes.find(name);
        if (it != prim.attributes.end())
        {
          const tinygltf::Accessor& acc = source.accessors[it->second];
          GetBufferData<Vec4>(source, acc, tangents);
        }
      }

      // uv's
      std::vector<Vec2> UVs;
      {
        std::string name = std::string("TEXCOORD_0");
        auto it = prim.attributes.find(name);
        if (it != prim.attributes.end())
        {
          const tinygltf::Accessor& acc = source.accessors[it->second];
          GetBufferData<Vec2>(source, acc, UVs);
        }
      }

      // merge them into the vertex vector
      vertices.reserve(positions.size()); // assume all vertices have a position
      for (int k = 0; k < positions.size(); ++k)
      {
        Vertex vertex;
        vertex.pos = positions[k];
        if (normals.size() > k) // account for missing normals
        {
          vertex.norm = normals[k];
        }
        else
        {
          vertex.norm = Vec3(0);
        }
        if (tangents.size() > k)
        {
          vertex.tang = tangents[k];
        }
        else
        {
          vertex.tang = Vec4(0);
        }
        if (UVs.size() > k)
        {
          vertex.uv = UVs[k];
        }
        else
        {
          vertex.uv = Vec2(0);
        }
        vertices.push_back(vertex);
      }
    }

    ////////////////////
    // Storage
    ////////////////////
    result.AddMesh(Mesh(indices, vertices, mat, l2w));
  }

  void Run(D3D11Renderer& renderer)
  {
    AssetManager manager(renderer);
    std::string file_name_cube = "../Assets/Samples/Cube/Cube.gltf";
    std::string file_name_duck = "../Assets/Samples/Duck/Duck.gltf";
    std::string file_name_axe = "../Assets/Samples/Hexagon/MS_Axe.glb";
    std::string err;

    // check initial load
    std::shared_ptr<Model> cube = manager.GetModel(file_name_cube, err);
    std::shared_ptr<Model> duck = manager.GetModel(file_name_duck, err);
    std::shared_ptr<Model> axe = manager.GetModel(file_name_axe, err);

    // check if you only get a reference, not a new copy
    std::shared_ptr<Model> cube2 = manager.GetModel(file_name_cube, err);
    std::shared_ptr<Model> duck2 = manager.GetModel(file_name_duck, err);
    std::shared_ptr<Model> axe2 = manager.GetModel(file_name_axe, err);

    int test = 0;
  }
}
