#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

int main(int argc,char* args[]){
	SDL_Surface* hello=NULL;
	SDL_Surface* screen=NULL;

    // initialize SDL video
    if (SDL_Init(SDL_INIT_EVERYTHING)<0){
        printf("Couldn't start SDL! %s\n",SDL_GetError());
        return 1;
    }
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	hello=SDL_LoadBMP("cb.bmp");

	SDL_BlitSurface(hello,NULL,screen,NULL);
	SDL_Flip(screen);
	SDL_Delay(2000);

	SDL_FreeSurface(hello);
	SDL_Quit();

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
