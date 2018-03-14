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
  std::shared_ptr<Model> GetModel(std::string& file, std::string& err);
  std::shared_ptr<Texture> GetTexture(std::string& file, std::string& err);

  void CleanUp();

  // test function
  void Run();
}