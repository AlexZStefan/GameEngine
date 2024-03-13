#pragma once
#include "Camera.h"
#include "Model.h"
#include "Structs.h"
#include "Texture.h"
#include "Functions.h"
#include "TerrainGenerator.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderUtils.h"
#include "Shader.h"
#include "VertexArray.h"
#include "BillboardRenderer.h"
#include "systems/ECS/Coordinator.h"

#include <glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <sstream>
// take out if not req 
#include <SDL.h>

namespace GE {
	enum SHADERS
	{
		STANDARD,
		TEXTURE,
		NORMALS,
		BILLBOARD,
		PARTICLE,
		GUI
	};

	class Renderer : public System
	{
	public:		
		/*Renderer(Shared<Shader> _shader);*/
		Renderer();

		virtual ~Renderer();

		void init();
		void update(Shared<Camera> cam);
		void setBillboardRend(bool renderB) { renderBillboard = renderB; };
		bool getBillboardRend() { return renderBillboard; };
				
		Shared<Billboard> getBillboard() {
			return model_bilboard; }

		void setBillboard(Shared<Billboard> blb_model, Vec3f pos) {
			model_bilboard = blb_model;
			model_bilboard->transform.position.x = pos.x ;
			model_bilboard->transform.position.y = pos.y;
			model_bilboard->transform.position.z = pos.z;
		}

		virtual void draw(Shared<Camera> cam);

	/*	Shared<Model> getModel() {
			return model;
		}*/

		Shared<Camera> camera;

		std::unordered_map<std::string, Shared<Shader>> shaders;

	protected:
		bool renderBillboard;

		GLuint programId;
		GLint vertexPosLocation;
		GLint vertexUVLocation;
		GLint vertexNormalLocation;

		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
		GLuint normalSamplerId;
				
		//std::shared_ptr<Model> model;
		//VertexBuffer* vb;
		//IndexBuffer* ibo;

		Shared<Billboard > model_bilboard;

	private:
		Entity entity;
	};
}