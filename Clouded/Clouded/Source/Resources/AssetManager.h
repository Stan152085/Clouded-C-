#pragma once
#include <unordered_map>
#include <string>
#include <memory>

// Stan's renderer
class D3D11Renderer;
// the handle to the model on the gpu
struct GPUModel;

// Rommie's transform class
class Transform;

// tinygltf's model
namespace tinygltf
{
  class Node;
  class Model;
  struct Primitive;
}

namespace resources
{
  // our texture class
  class Texture;
  // our model class
  class Model;

  class AssetManager
  {
  public:
    AssetManager(D3D11Renderer& renderer)
    :
      renderer_(renderer)
    {}
    /**
    * how to use:
    *   really floris you did not even bother to maintain the comment...
    */
    std::shared_ptr<Model> GetModel(std::string& file, std::string& err);
    std::shared_ptr<Texture> GetTexture(std::string& file, std::string& err);

    void CleanUp();

  private:
    // prevent copying
    AssetManager(const AssetManager& rhs);
    const AssetManager& operator=(const AssetManager& rhs);

    // helper functions to convert a tinygltf model into one of ours
    void RecursiveNodeExtract(const tinygltf::Model& source, const tinygltf::Node& node,
      const std::vector<std::shared_ptr<Texture>>& textures, Model& result, Transform& l2w);
    void ConstructModel(const tinygltf::Model& source, Model& result, const std::string& model_name);
    void ConstructMesh(const tinygltf::Model& source, const tinygltf::Primitive& prim,
      const std::vector<std::shared_ptr<Texture>>& textures, Model& result, const Transform& l2w);

    using ModelMap = std::unordered_map<std::string, std::shared_ptr<Model>>;
    using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;
    ModelMap model_map;
    TextureMap texture_map;
    D3D11Renderer& renderer_;
  };

  // test function
  void Run( D3D11Renderer& renderer);
}