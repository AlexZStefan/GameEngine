#include "FontRenderer.h"

namespace GE {
	FontRenderer::FontRenderer()
	{
	}
	
	void FontRenderer::loadFont(std::string text, std::string fontname, int size)
	{
		TTF_Init();
		font = TTF_OpenFont(fontname.c_str(), size);

		if (font == nullptr) {
			std::cout << "Font was not loaded " << TTF_GetError()<< std::endl;
		}
		SDL_Color white = { 255,255,255 };

		surfaceImage = TTF_RenderText_Solid(font, text.c_str(), white);

		// convert to rgba 
		convertedImage = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0);

		if (convertedImage != nullptr) {
			SDL_FreeSurface(surfaceImage);
			surfaceImage = convertedImage;
		}
		else {
			std::cout << "Error converting font " << text << ' ' << SDL_GetError() << std::endl;
		}

		int width = surfaceImage->w;
		int height = surfaceImage->h;

		Uint32 format = GL_RGBA;

		if (SDL_MUSTLOCK(surfaceImage)) {
			SDL_LockSurface(surfaceImage);
		}

		int bytesPerRow = width * surfaceImage->format->BytesPerPixel;
		char* temp = new char[bytesPerRow];

		for (int y = 0; y < height / 2; y++) {
			// copy bottom row into temp
			memcpy(temp, ((char*)surfaceImage->pixels) +((height - y - 1) * bytesPerRow), bytesPerRow);

			// copy top row to bottom row
			memcpy(((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), 
				(char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow);
			
			// copy temp to top row
			memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow);
		}

		delete[] temp;

		fontWidth = surfaceImage->w;
		fontHeight = surfaceImage->h;
		glGenTextures(1, &textureName);

		glBindTexture(GL_TEXTURE_2D, textureName);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, 
			format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(surfaceImage);
		
	}
	void FontRenderer::setText(std::string text)
	{
		
		SDL_Color white = { 255,255,255 };

		surfaceImage = TTF_RenderText_Solid(font, text.c_str(), white);

		// convert to rgba 
		convertedImage = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0);

		if (convertedImage != nullptr) {
			SDL_FreeSurface(surfaceImage);
			surfaceImage = convertedImage;
		}
		else {
			std::cout << "Error converting font " << text << ' ' << SDL_GetError() << std::endl;
		}

		int width = surfaceImage->w;
		int height = surfaceImage->h;

		Uint32 format = GL_RGBA;

		if (SDL_MUSTLOCK(surfaceImage)) {
			SDL_LockSurface(surfaceImage);
		}

		int bytesPerRow = width * surfaceImage->format->BytesPerPixel;
		char* temp = new char[bytesPerRow];

		for (int y = 0; y < height / 2; y++) {
			// copy bottom row into temp
			memcpy(temp, ((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), bytesPerRow);

			// copy top row to bottom row
			memcpy(((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow),
				(char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow);

			// copy temp to top row
			memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow);
		}

		delete[] temp;

		fontWidth = surfaceImage->w;
		fontHeight = surfaceImage->h;

		glBindTexture(GL_TEXTURE_2D, textureName);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, surfaceImage->pixels);
		
		SDL_FreeSurface(surfaceImage);

	}
}


