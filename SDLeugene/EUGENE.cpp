#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>

void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination);
bool init();
void cleanup();
bool load_files();
//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
//event structure
SDL_Event event;

int main(int argc,char *args[]){
	bool quit=false;

	if (init()==false){
		return 1;
	}
	if (load_files()==false){
		return 1;
	}
	//apply background to surface
	apply_surface(0,0,background,screen);
	//Apply the message to the screen
    apply_surface( 180, 140, message, screen );
    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
    	printf("no update");
        return 1;
    }
	while (quit==false){
		while (SDL_PollEvent(&event)){
			if (event.type==SDL_QUIT){
				quit=true;
			}
		}
	}
    //Quit SDL
    cleanup();
    return 0;
}
void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination){
	//temp rectangle to hold offsets
	SDL_Rect offset;
	//give offsets to rectangle
	offset.x=x;
	offset.y=y;

	//blit the surface
	SDL_BlitSurface( source, NULL, destination, &offset );
}
bool init(){
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return false;
	}
	//set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if (screen==NULL){
		return false;
    }
    //set window caption
	SDL_WM_SetCaption("Shutengu",NULL);
	//if all is well
	return true;
}
bool load_files(){
	//load images
	message=IMG_Load("cb.png");
	background=IMG_Load("horizon.png");
	return true;
}
void cleanup(){
	SDL_FreeSurface(message);
	SDL_FreeSurface(background);
	SDL_Quit();
}
