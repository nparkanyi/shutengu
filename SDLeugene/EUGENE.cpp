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


int main(){

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

}
