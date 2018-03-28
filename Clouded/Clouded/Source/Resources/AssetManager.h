#pragma once
#include <unordered_map>
#include <string>
#include <memory>

// Stan's renderer
class D3D11Renderer;
// the handle to the model on the gpu
struct GPUModel;

// tinygltf's model
namespace tinygltf
{
  class Model;
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
    AssetManager() {}
    /**
    * how to use:
    *   the first parameter is the file name. It has to be the complete relative path from
    the working directory to the model, including the file type.
    Example: std::string file_name_cube = "../Assets/Samples/Cube/Cube.gltf";

    The second parameter should be an empty string. If any errors occured during loading,
    they will be filled in here.

    The load functions return a shared pointer, so memory management is done automatically.
    There is always a reference in the resource maps in GLTFLoader.cpp, in global scope but
    only accesible from that file. If you want out dated resources to be deleted,
    call CleanUp(). It will remove all resources with just a single reference (the one in
    the map).
    */
    std::shared_ptr<GPUModel> GetModel(std::string& file, D3D11Renderer& renderer, std::string& err);
    std::shared_ptr<Texture> GetTexture(std::string& file, std::string& err);

    void CleanUp(D3D11Renderer& renderer);

  private:
    // prevent copying
    AssetManager(const AssetManager& rhs);
    const AssetManager& operator=(const AssetManager& rhs);

    // helper function to convert a tinygltf model into one of ours
    void ConstructModel(const tinygltf::Model& source, Model& result, const std::string& model_name);

    using ModelMap = std::unordered_map<std::string, std::shared_ptr<GPUModel>>;
    using TextureMap = std::unordered_map<std::string, std::shared_ptr<resources::Texture>>;
    ModelMap model_map;
    TextureMap texture_map;
  };

  // test function
  void Run(D3D11Renderer& renderer);
}