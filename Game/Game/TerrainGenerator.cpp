#include "TerrainGenerator.h"

namespace GE {

	GE::TerrainGenerator::TerrainGenerator(const std::string hMap, float widthS, float heightS)
	{
		heightMap = IMG_Load(hMap.c_str());

		if (heightMap == NULL) {
			std::cerr << "Could not load heightMap" << std::endl;
		}

		mapHeight		= heightMap->h;
		mapWidth		= heightMap->w;
		scaling			= 100.0f;
		heightScaling   = 10.0f;

		pixelData  = (unsigned char*)heightMap->pixels;
		components = heightMap->format->BytesPerPixel;

		for (int row = 0; row <= mapHeight; row++) {
			for (int column = 0; column < mapWidth; column++) {

				// coresponding value in the heightmap
				long int surfaceIdx    = ((row * mapWidth) + column) * components;

				unsigned char redPixel = pixelData[surfaceIdx];
				unsigned char greenPixel = pixelData[surfaceIdx+1];
				unsigned char bluePixel  = pixelData[surfaceIdx+2];
				// chars will be converted to int when evaluating the sum
				float heightValue = (float)((redPixel + greenPixel + bluePixel) / 3);

				float y = (heightValue / 255.0f)					* heightScaling;
				float x = ((float)column / (float)(mapWidth - 1))   * scaling;
				float z = ((float)row/ (float)(mapHeight- 1))		* scaling;

				u = (float)column / (float)(mapWidth - 1);
				v = (float)row / (float)(mapHeight - 1);

				vertices.push_back(Vertex(x, y, z, u, v));
			}
		}

		unsigned int c, n;
		for (int row = 0; row < mapHeight - 1; row++) {
			for (int column = 0; column < mapWidth - 1; column++) {
				c = row * mapWidth + column;
				n = (row + 1) * mapWidth + column;

				indices.push_back(c);
				indices.push_back(n);
				indices.push_back(c + 1);
				indices.push_back(c + 1);
				indices.push_back(n);
				indices.push_back(n + 1);
			}
		}

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);

		indexCount = indices.size();

		SDL_FreeSurface(heightMap);
	}

	GE::TerrainGenerator::~TerrainGenerator()
	{
	}

	void TerrainGenerator::bindIbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	void TerrainGenerator::unbindIbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}