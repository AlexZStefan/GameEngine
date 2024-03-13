#pragma once
#include "Model.h"
#include "Camera.h"
#include "EngineInterface.h"
#include <SDL.h>
#include "Externals/imgui/imgui.h"
#include "Externals/imgui/imgui_impl_sdl_gl3.h"
#include "PhysicsSystem.h"

namespace GE {

	class InputSystem
	{
	public:
		InputSystem(SDL_Event* evnt, Model* obj);
		InputSystem(SDL_Event* evnt, Shared<Camera> cam, Shared <PhysicsSystem> pSystem);

		~InputSystem();
		
		void Update(SDL_Window* window, float deltaTime);
		
		Entity GetSelectedEntity();
	private:
		SDL_Event* event; 
		Model* model;
		Shared<Camera> camera;
		Shared <PhysicsSystem> physicsSystem;
		float camSpeed = 20.0f;
		float mouseSensitivity = 30.0f;
		Entity selectedEntity;
		bool escapePressed;
	};

}

