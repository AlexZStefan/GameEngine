#pragma once
#include "Model.h"
#include "Structs.h"
#include "Billboard.h"

#include <SDL_image.h>
#include <string>
#include <iostream>

namespace GE {

	class TerrainGenerator: public Model
	{
	public:
		TerrainGenerator(const std::string hMap, float widthS, float heightS);
		// CreateFoliage(billboard, amount, size, yPosOffset);
		void CreateFoliage(Shared<Billboard>& bilboardTexture, int amount, int scale, int yOffset);

		~TerrainGenerator();

		void setTexture() {

		}
		
		void bindIbo();
		void unbindIbo();

		std::vector<Shared<Billboard>> getFoliage() {
			return blbL;
		}

		Shared<Texture> getTexture() {
			return bilboardTexture;
		}

	private:
		float scaling;
		float heightScaling;
		int components;
		unsigned char* pixelData;

		int mapWidth;
		int mapHeight;
		SDL_Surface* heightMap;
	
		float u, v; 

		Shared<Texture> bilboardTexture;
		std::vector<Shared<Billboard>> blbL;
		std::vector<Model*> modelL;
	};

}