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
SDL_Surface *up=NULL;
SDL_Surface *down=NULL;
SDL_Surface *left=NULL;
SDL_Surface *right=NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background=NULL;
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
	apply_surface(0,0,background, screen,NULL );
	while (quit==false){
		while (SDL_PollEvent(&event)){
			 if (event.type==SDL_QUIT){
				quit=true;
			}
		}
		Uint8 *keystates = SDL_GetKeyState( NULL );
		//If up is pressed
        if( keystates[ SDLK_UP ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen, NULL );
        }

        //If down is pressed
        if( keystates[ SDLK_DOWN ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen,NULL);
        }

        //If left is pressed
        if( keystates[ SDLK_LEFT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen,NULL );
        }

        //If right is pressed
        if( keystates[ SDLK_RIGHT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen,NULL );
        }
		if( keystates[ SDLK_UP ] && keystates[ SDLK_DOWN ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen, NULL );
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen,NULL);
        }
        if( keystates[ SDLK_UP ] && keystates[ SDLK_LEFT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen, NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen,NULL );
        }
        if( keystates[ SDLK_UP ] && keystates[ SDLK_RIGHT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen, NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen,NULL );
        }
        if( keystates[ SDLK_DOWN ] && keystates[ SDLK_RIGHT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen,NULL);
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen,NULL );
        }
        if( keystates[ SDLK_DOWN ] && keystates[ SDLK_LEFT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen,NULL);
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen,NULL );
        }
        if( keystates[ SDLK_LEFT ] && keystates[ SDLK_RIGHT ] )
        {
        	apply_surface(0,0,background, screen,NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen,NULL );
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen,NULL );
        }



		//Update the screen
			if( SDL_Flip( screen ) == -1 )
			{
				return 1;
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
	up=IMG_Load("arrowup.png");
	down=IMG_Load("arrowdown.png");
	left=IMG_Load("arrowleft.png");
	right=IMG_Load("arrowright.png");
	background=IMG_Load("horizon.png");

	return true;
}
void cleanup(){
	SDL_FreeSurface(up);
	SDL_FreeSurface(down);
	SDL_FreeSurface(left);
	SDL_FreeSurface(right);
	SDL_Quit();
}
