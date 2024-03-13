#pragma once
#include "Texture.h"
#include <string>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glew.h>
#include <GL/glew.h>

namespace GE {

	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer() {
			glDeleteTextures(1, &textureName);
			SDL_FreeSurface(convertedImage);
			
			TTF_CloseFont(font);
			TTF_Quit();
		};

		int getWidth() { return fontWidth; }
		int getHeight() { return fontHeight; }

		GLuint getFont() { return textureName; }
		// also sets the size of the font 
		void loadFont(std::string text, std::string fontname, int size);
		void setText(std::string text);

	private:
		GLuint textureName;

		SDL_Surface* surfaceImage;
		SDL_Surface* convertedImage;
		TTF_Font* font;

		int fontWidth;
		int fontHeight;
	};

}
