#pragma once
#include "Model.h"
#include "Camera.h"
#include <SDL.h>

namespace GE {

	class InputSystem
	{
	public:
		InputSystem(SDL_Event* evnt, Model* obj);
		InputSystem(SDL_Event* evnt, Camera *cam);

		~InputSystem();
		
		void movement(SDL_Window* window);
		
	private:
		SDL_Event* event; 
		Model* model;
		Camera* camera; 

		float camSpeed = 0.2f;
		float mouseSensitivity = 0.25f;
	};

}

