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
					camera->setPos(camera->getPos() + camera->getTarget()* camSpeed);
					break;
				case SDLK_s:
					camera->setPos(camera->getPos() - camera->getTarget()* camSpeed);
					break;
				case SDLK_a:
					camera->setPos(camera->getPos() - glm::normalize(glm::cross(camera->getTarget(), camera->getUpDir()))*camSpeed);
					break;
				case SDLK_d:
					camera->setPos(camera->getPos() + glm::normalize(glm::cross(camera->getTarget(), camera->getUpDir()))*camSpeed);
					break;
				case SDLK_SPACE:
					camera->setPosY(camera->getPosY() + 1);
					break;
				default:
					// 
					break;
				}

			if (event->type == SDL_MOUSEMOTION) {
					int mouse_x, mouse_y;
					float diffx, diffy;
					glm::vec3 direction;

					SDL_GetMouseState(&mouse_x, &mouse_y);
					diffx = mouse_x - camera->getMouseX();
					diffy = camera->getMouseY() - mouse_y;
					camera->setYaw((camera->getYaw() + diffx) * mouseSensitivity);
					camera->setPitch((camera->getPitch() + diffy) * mouseSensitivity);

					direction.x = cos(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
					direction.y = sin(glm::radians(camera->getPitch()));
					direction.z = sin(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
					camera->setTarget(glm::normalize(direction));

					std::cerr << "camera->getPitch()" << camera->getPitch() << std::endl;
					std::cerr << "camera->getYaw()" << camera->getYaw() << std::endl;
		
			}
		}

	}

}