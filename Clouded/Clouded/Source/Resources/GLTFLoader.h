#pragma once
#include <string>
#include <memory>

namespace tinygltf
{
  class Model;
}

namespace resources
{
  class Model;
  class Texture;

  std::shared_ptr<Model> GetModel(std::string& file, std::string& err);
  std::shared_ptr<Texture> GetTexture(std::string& file, std::string& err);
  void CleanUp();

	class GLTFLoader
	{
  public:

    void Run();
    void Clear();

  private:
		bool Load(std::string& file, tinygltf::Model& result, std::string& err);
	};
}