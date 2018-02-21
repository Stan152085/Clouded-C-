#pragma once
#include <string>

namespace tinygltf
{
  class Model;
}

namespace resources
{
	class GLTFLoader
	{
  public:

    void Run();
    void Clear();

  private:
		bool Load(std::string& file, tinygltf::Model& result, std::string& err);
	};
}