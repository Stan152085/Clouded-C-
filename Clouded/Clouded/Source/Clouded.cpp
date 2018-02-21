// Clouded.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "../../TinyGLTF/json.hpp"
#include "../../TinyGLTF/stb_image.h"
#include "../../TinyGLTF/tiny_gltf.h"

bool Load(std::string& file)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;
	std::string err;
	std::string ext = GetFilePathExtension(file);

	bool ret = false;
	if (ext.compare("glb") == 0) {
		std::cout << "Reading binary glTF" << std::endl;
		// assume binary glTF.
		ret = gltf_ctx.LoadBinaryFromFile(&model, &err, file.c_str());
	}
	else {
		std::cout << "Reading ASCII glTF" << std::endl;
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

int main()
{
	sf::Window window;
	sf::VideoMode mode(700, 500);

	window.create(mode, "Clouded");
	// game initialization

	while (window.isOpen())
	{
		// put main gameloop here
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
				break;
			}
		}
	}
    return 0;
}

