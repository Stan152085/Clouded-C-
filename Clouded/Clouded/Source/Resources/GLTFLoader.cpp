#include "stdafx.h"
#include "GLTFLoader.h"
#include "Model.h"
#include "Texture.h"
#include "ResourceManagementUtils.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef _DEBUG 
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#endif

#include "TinyGLTF/tiny_gltf.h"
#include <unordered_map>

using ModelMap = std::unordered_map<std::string, std::shared_ptr<resources::Model>>;
ModelMap model_map;
using TextureMap = std::unordered_map<std::string, std::shared_ptr<resources::Texture>>;
TextureMap texture_map;

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
  void GetVertexData(
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

  void ConstructModel(const tinygltf::Model& source, Model& result, const std::string& model_name)
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
    for (int i = 0; i < source.meshes.size(); ++i)
    {
      for (int j = 0; j < source.meshes[i].primitives.size(); ++j)
      {
        // each model might have multiple meshes
        const tinygltf::Primitive& prim = source.meshes[i].primitives[j];

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
          GetVertexData<unsigned short>(source, acc, indices);
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
              GetVertexData<Vec3>(source, acc, positions);
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
              GetVertexData<Vec3>(source, acc, normals);
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
              GetVertexData<Vec4>(source, acc, tangents);
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
              GetVertexData<Vec2>(source, acc, UVs);
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
        result.AddMesh(Mesh(indices, vertices, mat));
      }
    }
  }

  // Get a model from the cache. If it doesn't exist yet, load it
  std::shared_ptr<resources::Model> GetModel(std::string& file, std::string& err)
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
      return std::shared_ptr<resources::Model>(nullptr);
    }

    // construct the clouded model from the gltf model
    std::shared_ptr<Model> res(new Model());
    ConstructModel(gltf_model, *res, file);

    // insert the new model into the map
    model_map.insert(std::make_pair(file, res));
    return res;
  }

  // Get a texture from the cache. If it doesn't exist yet, load it
  std::shared_ptr<resources::Texture> GetTexture(std::string& file, std::string& err)
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

    // insert the new texture into the map
    texture_map.insert(std::make_pair(file, res));
    return res;
  }

  void CleanUp()
  {
    ModelMap::iterator model_it;
    for (model_it = model_map.begin(); model_it != model_map.end(); ++model_it)
    {
      if (model_it->second.use_count() < 2)
      {
        model_map.erase(model_it);
      }
    }
    model_map.clear();
    texture_map.clear();
  }

  void Run()
  {
    std::string file_name_cube = "../Assets/Samples/Cube/Cube.gltf";
    std::string file_name_duck = "../Assets/Samples/Duck/Duck.gltf";
    std::string err;

    tinygltf::Model model_cube;
    tinygltf::Model model_duck;

    Load(file_name_cube, model_cube, err);
    Load(file_name_duck, model_duck, err);

    Model res_cube;
    Model res_duck;
    ConstructModel(model_cube, res_cube, file_name_cube);
    ConstructModel(model_duck, res_duck, file_name_duck);

    std::shared_ptr<Model> cube = GetModel(file_name_cube, err);
    std::shared_ptr<Model> duck = GetModel(file_name_duck, err);

    int test = 0;
  }
}
