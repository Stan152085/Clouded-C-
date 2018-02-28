#include "stdafx.h"
#include "GLTFLoader.h"
#include "ResourceManagementUtils.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef _DEBUG 
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#endif

#include "TinyGLTF/tiny_gltf.h"


void resources::GLTFLoader::Run()
{
  std::string file_name_cube = "../Assets/Samples/Cube/Cube.gltf";
  std::string file_name_duck = "../Assets/Samples/Duck/Duck.gltf";
  std::string err;

  tinygltf::Model model_cube;
  tinygltf::Model model_duck;

  Load(file_name_cube, model_cube, err);
  Load(file_name_duck, model_duck, err);

  int test = 0;
  
  //////////////////////////////////////////////////
  // todo
  //   verify that these models are correct, and that the textures have loaded as well
  //   start designing a quick interface for this thing (does it have to be a class?)
  //   start on our own data format
  //   start on the resource management to avoid duplicates
  //////////////////////////////////////////////////
}

void resources::GLTFLoader::Clear()
{
}

bool resources::GLTFLoader::Load(std::string & file, tinygltf::Model& result, std::string& err)
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
