#include "stdafx.h"
#include "GLTFLoader.h"
#include "ResourceManagementUtils.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef _DEBUG 
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#endif

#include "TinyGLTF/tiny_gltf.h"


bool Load(std::string& file)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;
	std::string err;
	std::string ext = GetFilePathExtension(file);

	bool ret = false;
	if (ext.compare("glb") == 0) {
		//std::cout << "Reading binary glTF" << std::endl;
		// assume binary glTF.
		ret = gltf_ctx.LoadBinaryFromFile(&model, &err, file.c_str());
	}
	else {
		//std::cout << "Reading ASCII glTF" << std::endl;
		// assume ascii glTF.
		ret = gltf_ctx.LoadASCIIFromFile(&model, &err, file.c_str());
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