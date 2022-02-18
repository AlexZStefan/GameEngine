#include "InputSystem.h"

namespace GE {

	GE::InputSystem::InputSystem(SDL_Event* evnt, Model* obj)
	{
		model = obj;
		event = evnt;
	}

	GE::InputSystem::InputSystem(SDL_Event* evnt, Camera* cam)
	{
		camera = cam;
		event = evnt;
	}

	GE::InputSystem::~InputSystem()
	{

	}

	void GE::InputSystem::movement()
	{
		while (SDL_PollEvent(event))
		{
			if (event->type == SDL_KEYDOWN)
				switch (event->key.keysym.sym)
				{
				case SDLK_1:
					std::cerr << "SDASDAS" << std::endl;
					break;
				case SDLK_w:
					camera->setPosZ(camera->getPosZ() - 1);
					break;
				case SDLK_s:
					camera->setPosZ(camera->getPosZ() + 1);
					break;
				case SDLK_a:
					camera->setPosX(camera->getPosX() - 1);
					break;
				case SDLK_d:
					camera->setPosX(camera->getPosX() + 1);
					break;
				case SDLK_SPACE:
					camera->setPosY(camera->getPosY() + 1);
					break;
				case SDL_WINDOW_MOUSE_CAPTURE:
					std::cerr << "ASD MOUYSE" << std::endl;
 				default:
					// 
					break;
				}
		}

	}

}