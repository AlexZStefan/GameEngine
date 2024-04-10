#pragma once

#include "systems/ECS/Coordinator.h"
#include "Camera.h"
#include "InputSystem.h"
#include "SkyboxRenderer.h"
#include "TerrainGenerator.h"
#include "Texture.h"
#include "Renderer.h"
#include "Billboard.h"
#include "BillboardRenderer.h"
#include "Shader.h"
#include "GUIRenderer.h"
#include "Material.h"
#include "ParticleSystem.h"
#include "ObjectManager.h"
#include "PhysicsSystem.h"
#include "EngineInterface.h"
#include <functional>
#include <thread>
#include <SDL.h>
#include <GL/glew.h>
//#include <SDL2/SDL_op SDL_opengl.h>
//#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <ctime>
#include <unordered_map>
#include <map>

namespace GE {
	int static frame_rate; 

	class Time;
	enum GameModels {
		lowp_tree			,
		sacrifice_temple	,
		ship				,
		roman_warriors_blue ,
		roman_warriors_red	,
		sun					,
		wood				,
		eagle				,
		pavilion			,
		gtemple				,
		wood_cart			,
		horse				,
		rock				,
		new_cart			,
		terrain_model
	};

	enum Textures {
		terrain_texture ,
		tree_texture	,
		ship_uv			,
		roman_blue		,
		tree_billboard  ,
		roman_red		,
		text_sun		,
		text_wood		,
		normal_wood		,
		health_bar		,
		blb_grass		,
		t_eagle			,
		t_pavilion		,
		t_temple		,
		t_wood_cart		,
		t_horse			,
		t_rock			,
		blb_cloud		,
		blb_moon		,
		n_rock			,
		rain_drop		,
		t_cart			,
		terrain_color
	};
	
	class GameEngine {
	public:
		SDL_Surface* screen;
		// -1 addaptive Sync, 1 for updated sync with vertical retrace, 0 for immediate updates
		GameEngine(int setVSync);
		GameEngine(const GameEngine&);
		virtual ~GameEngine();

		int windowHeight = 960.0f;
		int windowWidth = 1280.0f;
		Time* times;
		bool init();
		bool initShaders();
		bool InitialiseSystems();
		bool keep_running();
		void update(float dt);
		void draw(float dt);
		void exit();
		void set_window_title(const char*);
		Shared<Shader> shader_basic_text;
		SDL_Window* window;
		std::unique_ptr<ObjectManager> objectManager;
		void PrintNumber();
		void AddModel(Vec3f position);
		void Test();
		void Raycast();

	private:
		std::vector<Entity> gameEntities;
		void load_models(std::string filePath);
		void load_textures(std::string filePath);

		bool initModels();
		void ModelTest();
		void createRenderer();

		void MultithreadingMainLoop(float dt);
		
		template<typename T>
		void AddGuiInspectorComponent(Entity entity);

		Shared<Camera> main_cam ;
		std::unique_ptr<InputSystem> inputSystem;
		SDL_Event input_event;
		SDL_GLContext glContext;
		
		int tick;
		int setVsync;

		Player player; 
		// Renderers
		std::unique_ptr<SkyboxRenderer> skyBoxRenderer;
		std::unique_ptr<BillboardRenderer> rend_billboard;
		std::unique_ptr<ParticleRenderer> rend_particles;
		std::unique_ptr<ParticleRenderer> clouds_particles;
		
		// Shaders
		Shared<Shader> shader_basic;
		Shared<Shader> shader_basic_normals;

		// Models
		Shared<TerrainGenerator> terrain;
		Shared<Billboard> blb;
		Shared<Billboard> b_rainDrop;
		Shared<Billboard> b_cloud;
		Shared<Billboard> b_grass;
		Shared<Billboard> b_moon;
		Shared<Billboard> LOD_tree;
		std::vector<Shared<Billboard>> foliage;

		GUIRenderer* GUI;
		FontRenderer* FPS; 

		std::vector<Shared<Model>> game_models;
		std::vector<Shared<Texture>> game_textures;
		std::vector<std::unique_ptr<Renderer>> game_renderers;

		Shared<Renderer> rendererSystem;
		Shared<PhysicsSystem> physicsSystem;
		
		// imGui windows
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};

	int time();
	void display_info_message(const char* msg);

	 class Time {
	public:
		Time() {
			std::time(&current_time);
			std::time(&last_time);
			seconds = 0;
		
		};

		void time() {
			std::time(&current_time);
			seconds = current_time - last_time ;
		}

		int tick()
		{
			int tick = SDL_GetTicks();
			return tick;
		}

		/*template <typename T>
		void waitFor(std::function<T()>* f, int seconds) {
			std::thread b([]() {
				std::this_thread::sleep_for(1000 * seconds)
				f();
				};
			if (b.joinable())
				b.join();
		}*/

		int getSeconds() { return seconds; }

	private:
		unsigned int start = 0;
		time_t current_time = 0;
		time_t last_time = 0;

		unsigned int seconds;

	
	};

	
	

}
