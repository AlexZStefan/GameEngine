#define SDL_MAIN_HANDLED

#include "GameEngine.h"
#include "SDL.h"   
#include <stdio.h>
#include <memory>
#include "ThreadPool.h"

namespace GE {
	static int frame_count = 0;
	static class Frame_Rate {
	public:
		int rate = frame_count;
	};
}

using namespace GE;


int main(int argc, char* argv[]) {
	GUIRenderer* GUI;
	FontRenderer* FPS;

	GameEngine gEngine = GameEngine(0);
	
	if (!gEngine.init()) {
		display_info_message("Could not start SDL. Check console for more details");
		return -1;
	}

	FPS = new FontRenderer();
	GUI = new GUIRenderer(gEngine.windowWidth, gEngine.windowHeight);

	GUI->addShader(gEngine.shader_basic_text);
	FPS->loadFont("HEllo", "./resources/assets/fonts/arial.ttf", 50);

	int current_time = SDL_GetTicks();
	int last_time = time();
	float delta = 0;
	float frame_second = 1 / 60;
	int every_second = 0;

	ThreadPool pool(std::thread::hardware_concurrency());

	while (gEngine.keep_running())
	{
		current_time = SDL_GetTicks();
		delta = static_cast<float>(current_time - last_time) / 1000.0f;
		if (delta > frame_second)
		{		
			last_time = current_time;
			gEngine.times->time();

			//pool.enqueue(std::bind(&gameEngine.update, delta));
			gEngine.update(delta);
			gEngine.draw(delta);

			// display framerate - update every second
			gEngine.shader_basic_text->SetUniform3f("Colour", 1, 1, 1);
			if (current_time - every_second > 1000)
			{
				every_second = current_time;
				//std::cout << "FPS : " << frame_count << std::endl;
				FPS->setText("FPS: " + std::to_string(frame_count));
				frame_count = 0;
			}

			// draw gui element 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GUI->drawText(gEngine.windowWidth - FPS->getWidth(), 0, FPS);
			glDisable(GL_BLEND);

			SDL_GL_SwapWindow(gEngine.window);

			frame_count++;

		}


		// display frames every second
		

	}
	delete GUI;
	delete FPS;

	gEngine.exit();

	return 0;

}