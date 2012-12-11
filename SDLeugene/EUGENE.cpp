#include "SDL/SDL.h"
#include <stdio.h>
#include <stdlib.h>
SDL_Surface *load_image(char *filename);
void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination);
//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;


int main(int argc,char *args[]){
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return 1;
	}
	//set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if (screen==NULL){
		return 1;
    }
    //set window caption
	SDL_WM_SetCaption("Shutengu",NULL);
	//load images
	message=load_image("cb.bmp");
	background=load_image("bg.bmp");
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
	//Wait 2 seconds
    SDL_Delay( 2000 );
    //Free the surfaces
    SDL_FreeSurface( message );
    SDL_FreeSurface( background );

    //Quit SDL
    SDL_Quit();

    //Return
    return 0;
}
SDL_Surface *load_image(char *filename){
	//Temporary storage for the image that's loaded
    SDL_Surface *loadedImage = NULL;
    //The optimized image that will be used
    SDL_Surface *optimizedImage = NULL;
    //Load the image
    loadedImage=SDL_LoadBMP(filename);
    if (loadedImage!=NULL){
		optimizedImage=SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
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
