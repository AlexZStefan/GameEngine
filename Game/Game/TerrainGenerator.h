#pragma once
#include "Model.h"
#include "Structs.h"

#include <SDL_image.h>
#include <string>
#include <iostream>
namespace GE {

	class TerrainGenerator: public Model
	{
	public:
		TerrainGenerator(const std::string hMap, float widthS, float heightS);

		~TerrainGenerator();

		void bindIbo();
		void unbindIbo();

	private:
		float scaling;
		float heightScaling;
		int components;
		unsigned char* pixelData;

		int mapWidth;
		int mapHeight;
		SDL_Surface* heightMap;
	
		float u, v; 

	};


}