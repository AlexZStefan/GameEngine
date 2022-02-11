#define SDL_MAIN_HANDLED

#include "GameEngine.h"
#include "SDL.h"   
#include <stdio.h>
#include <memory>

using namespace GE;

// macros defined in every headder file? 

int main(int argc, char* argv[]) {

	GameEngine gEngine = GameEngine(1);

	if (!gEngine.init()) {
		display_info_message("Could not start SDL. Check console for more details");
		return -1;
	}

	int current_time = time();
	int last_time = time();
	float delta = 0;
	int frame_second = 1000 / 60;
	int every_second = 0;

	int frame_count = 0;


	while (gEngine.keep_running())
	{
		current_time = time();
		delta = current_time - last_time;	

		if (delta > frame_second)
		{		
			gEngine.update();
			gEngine.draw();

			frame_count++;
			last_time = current_time;

		}


		// display frames every second
		if (current_time - every_second > 1000)
		{
			every_second = current_time;
			std::cout << "FPS : " << frame_count << std::endl;

			frame_count = 0;
		}

	}
	

	gEngine.exit();

	return 0;

}