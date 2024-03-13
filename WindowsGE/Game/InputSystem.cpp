#include "InputSystem.h"

namespace GE {
	extern Coordinator gCoordinator;
	extern EngineInterface engineInterface;

	GE::InputSystem::InputSystem(SDL_Event* evnt, Model* obj)
	{
		model = obj;
		event = evnt;
		escapePressed = false;
	}
	GE::InputSystem::InputSystem(SDL_Event* evnt, Shared<Camera> cam, Shared <PhysicsSystem> pSystem)
	{
		physicsSystem = pSystem;
		camera = cam;
		event = evnt;
	}
	GE::InputSystem::~InputSystem()
	{
	}

	bool rightMousePressed;

	void GE::InputSystem::Update(SDL_Window* window, float deltaTime)
	{
		int w, h;
		int mouse_x = 0;
		int mouse_y = 0;

		SDL_GetWindowSize(window, &w, &h);

		while (SDL_PollEvent(event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(event);
			if (event->type == SDL_KEYDOWN) {
				float cosPitch = cos(glm::radians(camera->getPitch()));
				switch (event->key.keysym.sym)
				{
				case SDLK_1:
					std::cerr << "SDASDAS" << std::endl;
					break;
				case SDLK_w:
					camera->setPos(camera->getPos() + camera->getTarget() * camSpeed * deltaTime);
					break;
				case SDLK_s:
					camera->setPos(camera->getPos() - camera->getTarget() * camSpeed * deltaTime);
					break;
				case SDLK_a:
					camera->setPos(camera->getPos() - glm::normalize(glm::cross(camera->getTarget(), camera->getUpDir())) * deltaTime * camSpeed);
					break;
				case SDLK_d:
					
					camera->setPos(camera->getPos() + glm::normalize(glm::cross(camera->getTarget(), camera->getUpDir())) * deltaTime * camSpeed);
					break;
				case SDLK_SPACE:
					camera->setPosY(camera->getPosY() + 1);
					break;
				case SDLK_LSHIFT:
					camSpeed *= 2;
					break;
				case SDLK_ESCAPE:
					escapePressed ? escapePressed = false :
						escapePressed = true;

					switch (escapePressed)
					{
					case true:
						SDL_ShowCursor(SDL_ENABLE);
						break;
					case false:
						SDL_ShowCursor(SDL_DISABLE);
						break;
					}
					break;
				default:
					// 
					break;
				}
			}
		
			if (event->type == SDL_MOUSEBUTTONDOWN) {
				if (event->button.button == SDL_BUTTON_RIGHT) {
					if (!rightMousePressed) {
						rightMousePressed = true;
						SDL_GetMouseState(&mouse_x, &mouse_y);
						camera->setOldMouseX(mouse_x);
						camera->setOldMouseY(mouse_y);
						SDL_SetWindowGrab(window, SDL_bool(rightMousePressed));

					}
				}
			}

			if (event->type == SDL_MOUSEBUTTONUP) {
				if (event->button.button == SDL_BUTTON_RIGHT) {
					if (rightMousePressed) {
						rightMousePressed = false;
						SDL_SetWindowGrab(window, SDL_bool(rightMousePressed));
					}
				}
			}

			if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
				engineInterface.EntityRaycast(mouse_x, mouse_y, w, h);
			}

			if (rightMousePressed && SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
					glm::vec3 direction{ 0,0,0 };
					
					float mouseDeltaX = ((mouse_x - camera->getOldMouseX()));
					float mouseDeltaY = ((mouse_y - camera->getOldMouseY()));

					camera->setOldMouseX(mouse_x);
					camera->setOldMouseY(mouse_y);

					mouseDeltaX *= deltaTime * mouseSensitivity;
					mouseDeltaY *= deltaTime * mouseSensitivity;
					//std::cout << "new yaw " << (camera->getYaw() + mouseDeltaX) << "\n";
					//std::cout << "mouseDeltaY " << mouseDeltaY << "\n";
					camera->setYaw((camera->getYaw() + mouseDeltaX));
					camera->setPitch((camera->getPitch() - mouseDeltaY));
					// calculate angles
					direction.x = cos(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
					direction.y = sin(glm::radians(camera->getPitch()));
					direction.z = sin(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));

					camera->setTarget(glm::normalize(direction));



					/* Buggy
					//std::cout << camera->camTarget().x << " " << camera->camTarget().y << " " << camera->camTarget().z << "\n";

					//int midX = w / 2, midY = h / 2;
					////SDL_SetWindowGrab(SDL_Window * window, SDL_bool grabbed);
					//glm::vec3 direction;

					//int mouseDeltaX = -(mouse_x - camera->getOldMouseX());
					//int mouseDeltaY = (mouse_y - camera->getOldMouseY());

					//camera->setOldMouseX(mouse_x);
					//camera->setOldMouseY(mouse_y);

					//std::cout << "dx " << mouseDeltaX << " y " << mouseDeltaY << "\n";
					//camera->setYaw(camera->getYaw() + mouseDeltaX );
					//camera->setPitch(camera->getPitch() + mouseDeltaY );

					//// calculate angles
					//direction.x = cos(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
					//direction.y = sin(glm::radians(camera->getPitch()));
					//direction.z = sin(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));

					// if gimbal lock try this
					/*float cosPitch = cos(glm::radians(camera->getPitch()));
					direction.x = cosPitch * sin(glm::radians(camera->getYaw()));
					direction.y = sin(glm::radians(camera->getPitch()));
					direction.z = cosPitch * cos(glm::radians(camera->getYaw()));

					camera->setTarget(glm::normalize(direction));
					*/
					//std::cerr << "SDL_GetMouseState(&mouse_x, &mouse_y);" << w <<"h"<< h << " " << mouse_y << std::endl;
					//std::cerr << "(&mouse_x,);" << camera->getMouseX() << std::endl;
					//std::cerr << "camera->getYaw()" << camera->getYaw() << std::endl;

					//SDL_WarpMouseInWindow(window, w/2, h/2);
				}
		}

	}

	Entity InputSystem::GetSelectedEntity()
	{
		return selectedEntity;
	}

}