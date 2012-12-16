#include <"SDL/SDL_image.h">
#include <"SDL/SDL.h">
#include <string>
#include <sstream>

int main (int argc, char *args[]){
	//quit flag
	bool quit=false;

	//timer starter
	Uint32 start=0;

	//timer start/stop flag
	bool running=true;

	//start timer
	start=SDL_GetTicks();

	//while user hasn't quit
	while (quit==false){
		//while events exist
		while (SDL_PollEvent(&event)){
			//if key pressed
			if (event.type==SDL_KEYDOWN){
				//if "s" was pressed
				if (event.key.keysym.sym==SDLK_s){
					//if timer running
					if (running==true){
						//stop timer
						running = false
						start=0;
					}
					else {
						//start timer
						running =true;
						start=SDL_GetTicks();
					}
				}
			}
			//if timer is running
			if (running==true){
				//timer's time as string
				std::stringstream time;
				//conver timer's time to a string
				time <<"Timer: "<<SDL_GetTicks()-start;
			}
		}
	}
}
