#include "TerrainGenerator.h"

namespace GE {

	GE::TerrainGenerator::TerrainGenerator(const std::string hMap, float widthS, float heightS)
	{
		bilboardTexture = std::make_shared<Texture>("./resources/assets/textures/tree_billboard.png");

		heightMap = IMG_Load(hMap.c_str());

		if (heightMap == NULL) {
			std::cerr << "Could not load heightMap" << std::endl;
		}

		mapWidth		= heightMap->w;
		mapHeight		= heightMap->h;
		scaling			= 1000.0f;
		heightScaling   = 100.0f;

		pixelData  = (unsigned char*)heightMap->pixels;
		components = heightMap->format->BytesPerPixel;

		for (int row = 0; row <= mapHeight; row++) {

			//int thisC = (int)rand() % mapHeight;

			for (int column = 0; column < mapWidth; column++) {

				// coresponding value in the heightmap
				long int surfaceIdx    = ((row * mapWidth) + column) * components;

				/*unsigned char redPixel = pixelData[surfaceIdx];
				unsigned char greenPixel = pixelData[surfaceIdx+1];
				unsigned char bluePixel  = pixelData[surfaceIdx+2];*/
				// chars will be converted to int when evaluating the sum
				//float heightValue = (float)((redPixel + greenPixel + bluePixel) / 3);
				
				float heightValue = (float)(pixelData[surfaceIdx]);

				float y = (heightValue / 255.0f)					* heightScaling;
				float x = ((float)column / (float)(mapWidth - 1))   * scaling;
				float z = ((float)row/ (float)(mapHeight- 1))		* scaling;

				u = (float)column / (float)(mapWidth - 1);
				v = (float)row / (float)(mapHeight - 1);

				//TODO Compute normal for the face and add

				vertices.push_back(Vertex(x, y, z, u, v, 1.0f, 1.0f, 1.0f));
				//// Scatter trees accross map 
				//if (blbL.size() < 20 && ) {
				///*	std::cout << "heightMap w" << heightMap->w << std::endl;
				//	std::cout << "heightMap h" << heightMap->h << std::endl;*/

				//Shared<Billboard> temp = std::make_shared<Billboard>(bilboardTexture);

				//float xT = ((float)(rand() * column % heightMap->w));
				//float zT = ((float)(rand() * column % heightMap->h));
				//float yT = ((float)(rand() * column % heightMap->h));

				//temp->position = { xT,yT, zT };

				//std::cout << "temp X" << temp->position.x << " z :" << temp->position.z << std::endl;

				//temp->scale = Vec3f( 10.0f, 10.0f, 10.0f );
				//blbL.push_back(temp);
				//
				//}
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

		indexCount = indices.size();
		numVertices = vertices.size();
		SDL_FreeSurface(heightMap);
	}

	void GE::TerrainGenerator::CreateFoliage(Shared<Billboard> &bilboardTexture, int amount,int scale, int yOffset) {
		int index = (int)rand() % numVertices;
		int offset;

		for(int i = 0; i < amount; i++)
		{
			offset = ((((int)rand()*10) % 10000)* index) % numVertices;
			Shared<Billboard> temp = std::make_shared<Billboard>(bilboardTexture->getTexture());
			Vertex tempPosition = vertices[offset];
			temp->transform.position = { tempPosition.x,tempPosition.y-yOffset,  tempPosition.z};

			std::cout << "temp X" << temp->transform.position.x << " z :" << temp->transform.position.z << std::endl;

			temp->transform.scale = Vec3f(1.0f*  scale, 1.0f * scale, 1.0f * scale);
			blbL.push_back(temp);

		}
	}
	

	GE::TerrainGenerator::~TerrainGenerator()
	{
	
	}

	void TerrainGenerator::bindIbo()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	void TerrainGenerator::unbindIbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}