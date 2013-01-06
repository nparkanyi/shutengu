//Jon Liu, Eugene Wang, Ms. Odecki, ICS3U, January 2013
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//screen attributes
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;
const int FRAMES_PER_SECOND=60;

//ship attributes
const int SHIP_WIDTH=40;
const int SHIP_HEIGHT=50;
const int SHIP_SPEED=3;

//surfaces starring in this production
SDL_Surface *menu=NULL;
SDL_Surface *theship=NULL;
SDL_Surface *background=NULL;
SDL_Surface *boss=NULL;
SDL_Surface *screen=NULL;
SDL_Surface *menumsg=NULL;

//the award-winning soundtrack
Mix_Music *bgm=NULL;
Mix_Chunk *gain=NULL;
Mix_Chunk *bomb=NULL;

//fonts and colours
TTF_Font *menuFont=NULL;
SDL_Color defaultColor={0,0,0};

//event handler
SDL_Event event;

//lists ship properties
class ship{
    private:
		int x, y;			//position
		int xVel, yVel;		//velocity

    public:
		ship();				//initializes
		void handle_input();//handles keyboard controls for the ship
		void move();		//handles motion
		void show();		//renders ship
};

//lists timer properties
class Timer{
    private:
		int startTicks;		//time when it started
		int pausedTicks;	//when paused: time past since start
		bool paused;		//see isPaused
		bool started;		//see isStarted

    public:
		Timer();			//initializes
		void start();		//timer controls
		void stop();
		void pause();
		void unpause();
		int getTicks();	//time past in the timer
		bool isStarted();	//timer status
		bool isPaused();
};

//renders surfaces in specified coordinate
void printb(int xpos,int ypos,SDL_Surface* orig,SDL_Surface* target,SDL_Rect* clip=NULL){
    SDL_Rect offset;		//stores coordinates
    offset.x = xpos;
    offset.y = ypos;

	//render
    SDL_BlitSurface(orig,clip,target,&offset);
}

//initializes SDL subsystems
bool init(){
    //all SDL defaults or return false if error
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1) return false;

    //init screen or return false if error
    screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
    if(screen==NULL) return false;

    //init SDL_mixer or return false if error
    if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1) return false;

	//init SDL_ttf or return false if error
	if(TTF_Init()==-1) return false;

    SDL_WM_SetCaption("Shutengu 0.8",NULL);	//set window caption
    SDL_ShowCursor(SDL_DISABLE);			//hide mouse pointer

    //all is well if we reach this point
    return true;
}

//initializes most game assets
bool prepAssets(){
    //load images or return false if error
    menu=IMG_Load("img/menu.png");
    theship=IMG_Load("img/gameship.png");
    background=IMG_Load("img/background.png");
    boss=IMG_Load("img/boss.png");
    if((menu==NULL)||(theship==NULL)||(background==NULL)||(boss==NULL)) return false;

	//load audio or return false if error
	bgm=Mix_LoadMUS("audio/hahaha.wav");	//menu music by default
	gain=Mix_LoadWAV("audio/1up.wav");
	bomb=Mix_LoadWAV("audio/boom.wav");
	if((bgm==NULL)||(gain==NULL)||(bomb==NULL)) return false;

	//open fonts or return false if error
	menuFont=TTF_OpenFont("envy.ttf",16);
	if((menuFont==NULL)) return false;

    //all is well
    return true;
}

//unloads assets and closes SDL subsystems
void cleanUp(){
    //free all surfaces
    SDL_FreeSurface(menu);
    SDL_FreeSurface(theship);
    SDL_FreeSurface(background);
    SDL_FreeSurface(boss);
    SDL_FreeSurface(menumsg);
    SDL_FreeSurface(screen);

	//free all audio
	Mix_FreeMusic(bgm);
	Mix_FreeChunk(gain);
	Mix_FreeChunk(bomb);
	Mix_CloseAudio();

	//close all fonts
	TTF_CloseFont(menuFont);
	TTF_Quit();

    //quit SDL
    SDL_Quit();
}

//randomly determines new music to play, then plays it
bool newBGM(){
	srand(time(NULL));
	int rMus=rand()%9+1;	//from 1 to 9

	//randomly picks new music form this pool
	switch(rMus){
		case 1: bgm=Mix_LoadMUS("audio/arps.wav"); break;
		case 2: bgm=Mix_LoadMUS("audio/chase1.wav"); break;
		case 3: bgm=Mix_LoadMUS("audio/chase2.wav"); break;
		case 4: bgm=Mix_LoadMUS("audio/gacd.wav"); break;
		case 5: bgm=Mix_LoadMUS("audio/train.wav"); break;
		case 6: bgm=Mix_LoadMUS("audio/hey.wav"); break;
		case 7: bgm=Mix_LoadMUS("audio/juice.wav"); break;
		case 8: bgm=Mix_LoadMUS("audio/partybell.wav"); break;
		case 9: bgm=Mix_LoadMUS("audio/pounds.wav"); break;
	}

	//play it or return false if error
	if(Mix_PlayMusic(bgm,-1)==-1) return false;

	//all is well
	return true;
}

//initializes ship's properties
ship::ship(){
    x = 300;		//ship's initial position
    y = 400;
    xVel = 0;		//ship's initial velocity
    yVel = 0;		//of course it's zero
}

//responds to keypresses involving the ship
void ship::handle_input(){
    //key is pressed
    if(event.type==SDL_KEYDOWN){
		switch(event.key.keysym.sym){
			case SDLK_UP: 		yVel-=SHIP_SPEED; break;
			case SDLK_DOWN: 	yVel+=SHIP_SPEED; break;
			case SDLK_LEFT: 	xVel-=SHIP_SPEED; break;
			case SDLK_RIGHT: 	xVel+=SHIP_SPEED; break;
		}
	}

    //key is released
	if(event.type==SDL_KEYUP){
		switch(event.key.keysym.sym){
			case SDLK_UP: 		yVel +=SHIP_SPEED; break;
			case SDLK_DOWN: 	yVel -=SHIP_SPEED; break;
			case SDLK_LEFT: 	xVel +=SHIP_SPEED; break;
			case SDLK_RIGHT: 	xVel -=SHIP_SPEED; break;
		}
    }
}

//controls ship movement
void ship::move(){
    //horizontal motion: stop the ship if out of bounds
    x+=xVel;
    if((x<120)||(x+SHIP_WIDTH>520)) x-=xVel;

    //vertical motion: stop the ship if out of bounds
    y+=yVel;
    if((y<0)||(y+SHIP_HEIGHT>SCREEN_HEIGHT)) y-=yVel;
}

//renders the ship
void ship::show(){
    printb( x, y, theship, screen );
}

//init timer properties
Timer::Timer(){
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

//starts the timer
void Timer::start(){
	started = true;				//it is now running
	paused = false;				//it is not paused
	startTicks = SDL_GetTicks();//time when timer was started
}

//clears and resets the timer
void Timer::stop(){
	started = false;			//no longer running
	paused = false;
}

//pauses timer increment
void Timer::pause(){
	//run only if timer is running
	if(started==true&&paused==false){
		paused = true;			//now paused
		//once paused: time past since started
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

//resumes timer increment
void Timer::unpause(){
	//run only if timer is paused
	if(paused==true){
		paused = false;			//no longer paused
		//continue counting time
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

//finds time since timer start
int Timer::getTicks(){
	//run only if timer is active
	if(started==true){
		if(paused==true) return pausedTicks;
		else return(SDL_GetTicks()-startTicks);
	}

	//at this point: timer isn't running
	return 0;
}

//checks if timer is active
bool Timer::isStarted(){
	return started;
}

//checks if timer is not incrementing
bool Timer::isPaused(){
	return paused;
}

int main(int argc,char* args[]){
	FILE *fptr;
	char msg[100];
	if ((fptr=fopen("text/swkey.txt","r"))!=NULL){
		if (fgets(msg,100,fptr)==NULL){
			printf("error");
		}
		fclose(fptr);
	}
	menumsg=TTF_RenderText_Solid(menuFont,"asdasdsaasdsaads",defaultColor);

	//setup
    bool quit = false;		//maintains program loop
    ship myship;			//user-controlled ship
	Timer fps;				//controls frame rate
	Timer wave;				//time until next wave
    if(init()==false) return 1;
    if(prepAssets()==false) return 1;
	if(Mix_PlayMusic(bgm,-1)==-1) return 1;
	fps.start();
	wave.start();

    //game runs here
    while(quit==false){
    	//MENU HERE

		//once wave time is up: level up and restart wave timer
        if (wave.getTicks()>5000){
			wave.stop();
			newBGM();
			//WAVE CHANGE HERE
			wave.start();
		}

        //while there's science to do
        while(SDL_PollEvent(&event)){
			//ship controls
            myship.handle_input();

            //other controls
            if(event.type==SDL_KEYDOWN){
				switch( event.key.keysym.sym ){
					case SDLK_ESCAPE:
						quit=true; break;
					case SDLK_x:
						if(Mix_PlayChannel(-1,bomb,0)==-1) return 1; break;
					case SDLK_z:
						menu=NULL;
				}
			}

			//if the window gets X'd
            if( event.type == SDL_QUIT ){
                quit = true;
            }
        }

        //update ship's position
        myship.move();
        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        printb(0,0,background,screen);
        printb(0,0,boss,screen);
        myship.show();
        printb(0,0,menu,screen);
        printb(200,200,menumsg,menu);

        //refresh the screen
        if(SDL_Flip(screen)==-1) return 1;

        //limit the frame rate
        if(fps.getTicks()<1000/FRAMES_PER_SECOND){
            SDL_Delay((1000/FRAMES_PER_SECOND)-fps.getTicks());
        }
    }

    //user has now quit
    cleanUp();

    return 0;
}
