#include "GameEngine.h"

namespace GE{


GE::GameEngine::GameEngine(int setVSync)
{


	setVsync = setVSync;
}

GE::GameEngine::~GameEngine()
{
}

bool GE::GameEngine::init()
{
	/* Initialize the SDL library */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// Set OpenGL version to use core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// Set type of profile which is core meaning modern OpenGL
	// no legacy features for backword compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("ZarEngine", 50, 50, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "Unable to create window." << SDL_GetError() << std::endl;
	return false;
	}

	// Create the OpenGL context and links this to window object
	// Context represents OpenGL for program such as objects and drawing
	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		std::cerr << "SDL could not create GL context " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialize GLEW - solves problem with OPENGL on window
	// GLEW binds latest extensions that can be used; default API from `97
	
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Error initialising GLEW " << glewGetErrorString(status) << std::endl;
		return false;
	}

	
	if (SDL_GL_SetSwapInterval(setVsync) != 0)
	{
		std::cerr << "Unable to set VSync!" << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "SDL Init successful " << std::endl;


	// init Camera
	// Init Triangle Renderer
	renderer = new Renderer();
	renderer->init();

	return true;
}

bool GE::GameEngine::keep_running()
{
	SDL_PumpEvents();

	SDL_Event evt;

	if (SDL_PeepEvents(&evt,1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT))
	{
		return false;
	}

	return true;
}

int time()
{
	int tick = SDL_GetTicks();

	return tick;
}

void GE::GameEngine::update()
{
	renderer->update();

}

void GE::GameEngine::draw()
{
	glClearColor(0.4f, 0.5f, 0.9, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer->draw();


	//glBegin(GL_POLYGON);
	//glColor3f(1.f, 1.f, 0.f);
	//glVertex2f(-1.0f, 0.f);
	//glColor3f(1.f, 1.f, 0.f);
	//glVertex2f(1.0f, 0.f);
	//glColor3f(1.f, 1.f, 0.f);
	//glVertex2f(.0f, 1.f);


	//glEnd();


	SDL_GL_SwapWindow(window);
}

void GE::GameEngine::exit()
{
	renderer->destroy();


	SDL_DestroyWindow(window);

	this->~GameEngine();
	
	window = nullptr;

	SDL_Quit();

}

void GE::GameEngine::set_window_title(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);

}

void display_info_message(const char* msg) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "BestGameEngine", msg, nullptr);
}

}