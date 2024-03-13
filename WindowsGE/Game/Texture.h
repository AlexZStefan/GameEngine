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
		std::string name;
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
		GLuint getNormalMap() {
			return normalID;
		}
		void loadTexture(std::string filename);
		void loadNormalMap(std::string filename);
		void setHeight(int h) {
			height = h;
		}
		void setWidth(int w) {
			width = w;
		}
		GLuint textureName;
	private:

		int width;
		int height;
		Uint32 format;
		GLuint normalID;
	};
}
