#pragma once
#include <iostream>
#include <SDL_image.h>
#include <vector>
#include <map>
#include <string>
#include <glew.h>

namespace GE {
	class Texture
	{
	public:
		Texture(std::string filename);
		~Texture();

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height; 
		}

		Uint32 getFormat() {
			return format;
		}

		GLuint getTexture() {
			return textureName;
		}

		void addTextures();

	private:
		void loadTexture(std::string filename);

		int width;
		int height;
		Uint32 format;
		GLuint textureName;

	
		std::vector<std::map<std::string, SDL_Texture*>> textures; 
	};
}
