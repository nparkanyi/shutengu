#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>

void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination,SDL_Rect *clip);
bool init();
bool load_files();
void cleanup();

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *dots=NULL;
SDL_Surface *screen = NULL;
//event structure
SDL_Event event;
//portion of sprite map to be blitted
SDL_Rect clip[4];

int main(int argc,char *args[]){
	bool quit=false;

	if (init()==false){
		return 1;
	}
	if (load_files()==false){
		return 1;
	}
	//clip range for top left
	clip[0].x=0;
	clip[0].y=0;
	clip[ 0 ].w = 100;
    clip[ 0 ].h =100;

    //Clip range for the top right
    clip[ 1 ].x = 100;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 100;
    clip[ 1 ].h = 100;

    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 100;
    clip[ 2 ].w = 100;
    clip[ 2 ].h = 100;

    //Clip range for the bottom right
    clip[ 3 ].x = 100;
    clip[ 3 ].y = 100;
    clip[ 3 ].w = 100;
    clip[ 3 ].h = 100;

    //fill screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    //Apply the images to the screen
    apply_surface( 0, 0, dots, screen, &clip[ 0 ] );
    apply_surface( 540, 0, dots, screen, &clip[ 1 ] );
    apply_surface( 0, 380, dots, screen, &clip[ 2 ] );
    apply_surface( 540, 380, dots, screen, &clip[ 3 ] );

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
    	printf("no update");
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
void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip=NULL){
	//temp rectangle to hold offsets
	SDL_Rect offset;
	//give offsets to rectangle
	offset.x=x;
	offset.y=y;

	//blit the surface
	SDL_BlitSurface( source, clip, destination, &offset );
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
	dots=IMG_Load("circle.png");
	if(dots==NULL){
		printf("Couldn't load the image!\n");
		return false;
	}

	return true;
}
void cleanup(){
	SDL_FreeSurface(dots);
	SDL_Quit();
}
