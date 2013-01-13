//Jon Liu, Eugene Wang, Ms. Odecki, ICS3U, January 2013
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>

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
SDL_Surface *bullet=NULL;
SDL_Surface *points=NULL;

//the award-winning soundtrawhile(SDL_PollEvent(&event)){ck
Mix_Music *bgm=NULL;
Mix_Chunk *gain=NULL;
Mix_Chunk *bomb=NULL;

//fonts and colours
TTF_Font *menuFont=NULL;
SDL_Color defaultColor={0,0,0};
SDL_Color menubgColor={0xff,0xff,0xff};
SDL_Color pointsColor={0xaf,0xdd,0xe9};

//event handler
SDL_Event event;

//lists ship properties
class ship{
    private:
		int xVel, yVel;		//velocity

    public:
		SDL_Rect hitbox;
		ship();				//initializes
		void handle_input();//handles keyboard controls for the ship
		void move();		//handles motion
		void show();		//renders ship
};

struct bulletData{
	SDL_Rect hitbox;
	int xVel;
	int yVel;
};

bulletData b[5];

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

    int i;

    //load images or return false if error
    menu=IMG_Load("img/menu.png");
    theship=IMG_Load("img/gameship.png");
    background=IMG_Load("img/background.png");
    boss=IMG_Load("img/boss.png");
    bullet=IMG_Load("img/bullet.png");
    if((menu==NULL)||(theship==NULL)||(background==NULL)||(boss==NULL)) return false;

	//load audio or return false if error
	bgm=Mix_LoadMUS("audio/hahaha.wav");	//menu music by default
	gain=Mix_LoadWAV("audio/1up.wav");
	bomb=Mix_LoadWAV("audio/boom.wav");
	if((bgm==NULL)||(gain==NULL)||(bomb==NULL)) return false;

	//open fonts or return false if error
	menuFont=TTF_OpenFont("envy.ttf",24);
	if((menuFont==NULL)) return false;

    for (i=0;i<5;i++)
    {
        b[i].hitbox.x=rand()%420+120;
		b[i].hitbox.h=5;
		b[i].hitbox.w=5;
<<<<<<< HEAD
		b[i].hitbox.y=0;
=======
		b[i].hitbox.y=rand()%20+20;
>>>>>>> ef7ab364672f35b2cc1abe0682778585a4806fe7
    }

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
    SDL_FreeSurface(bullet);

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

//checks collision
bool isCol(SDL_Rect rectA,SDL_Rect rectB){
	int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA=rectA.x;
    rightA=rectA.x+rectA.w;
    topA=rectA.y;
    bottomA=rectA.y+rectA.h;

    leftB=rectB.x;
    rightB=rectB.x+rectB.w;
    topB=rectB.y;
    bottomB=rectB.y+rectB.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

//initializes ship's properties
ship::ship(){
	hitbox.x=300;
	hitbox.y=400;
	hitbox.h=SHIP_HEIGHT;
	hitbox.w=SHIP_WIDTH;
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
    hitbox.x+=xVel;
    if((hitbox.x<120)||(hitbox.x+SHIP_WIDTH>520)) hitbox.x-=xVel;

    //vertical motion: stop the ship if out of bounds
    hitbox.y+=yVel;
    if((hitbox.y<0)||(hitbox.y+SHIP_HEIGHT>SCREEN_HEIGHT)) hitbox.y-=yVel;
}

//renders the ship
void ship::show(){
    printb( hitbox.x, hitbox.y, theship, screen );
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
    srand(time(NULL));
	//setup
	bool quitMenu=false;	//maintains menu loop
    bool quitGame=false;	//maintains game loop
    ship myship;			//user-controlled ship
	Timer fps;				//controls frame rate
	Timer wave;				//time until next wave
	Timer scoreT;
    if(init()==false) return 1;
    if(prepAssets()==false) return 1;
	if(Mix_PlayMusic(bgm,-1)==-1) return 1;
<<<<<<< HEAD

	//read and display the string of appropriate language
	FILE *fptr;
	char strFile[25];
	if((fptr=fopen("text/gr.txt","r"))!=NULL){
		if(fgets(strFile,25,fptr)==NULL) return 1;
	}
	fclose(fptr);
	menumsg=TTF_RenderText_Shaded(menuFont,strFile,defaultColor,menubgColor);

	//menu runs here
	while(quitMenu==false){
		//display menu
        printb((SCREEN_WIDTH-menumsg->w)/2,400,menumsg,menu);
        printb(0,0,menu,screen);

		//menu-only key controls
		while(SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                switch( event.key.keysym.sym ){
                    case SDLK_RETURN: quitMenu=true; break;
                    case SDLK_ESCAPE:
						quitMenu=true;
						quitGame=true; break;
                }
            }

			//if the window gets X'd
			if(event.type==SDL_QUIT){
					quitMenu=true;			//quit the menu
					quitGame=true;			//skip the game
			}
		}

		//refresh the screen
        if(SDL_Flip(screen)==-1) return 1;
	}

	wave.start();
	scoreT.start();
	newBGM();

    //game runs here
    while(quitGame==false){
		fps.start();
		//once wave time is up: level up and restart wave timer
        if (wave.getTicks()>10000){
			newBGM();
			//WAVE CHANGE/TIMER RESTART HERE
			wave.start();
		}

=======

	//read and display the string of appropriate language
	FILE *fptr;
	char strFile[25];
	if((fptr=fopen("text/gr.txt","r"))!=NULL){
		if(fgets(strFile,25,fptr)==NULL) return 1;
	}
	fclose(fptr);
	menumsg=TTF_RenderText_Shaded(menuFont,strFile,defaultColor,menubgColor);

	//menu runs here
	while(quitMenu==false){
		//display menu
        printb((SCREEN_WIDTH-menumsg->w)/2,400,menumsg,menu);
        printb(0,0,menu,screen);

		//menu-only key controls
		while(SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                switch( event.key.keysym.sym ){
                    case SDLK_RETURN: quitMenu=true; break;
                    case SDLK_ESCAPE:
						quitMenu=true;
						quitGame=true; break;
                }
            }

			//if the window gets X'd
			if(event.type==SDL_QUIT){
					quitMenu=true;			//quit the menu
					quitGame=true;			//skip the game
			}
		}

		//refresh the screen
        if(SDL_Flip(screen)==-1) return 1;
	}

	wave.start();
	scoreT.start();
	newBGM();

    //game runs here
    while(quitGame==false){
		fps.start();
		//once wave time is up: level up and restart wave timer
//        if (wave.getTicks()>10000){
//			newBGM();
//			//WAVE CHANGE/TIMER RESTART HERE
//			wave.start();
//		}

>>>>>>> ef7ab364672f35b2cc1abe0682778585a4806fe7
		//while there's science to do
		while(SDL_PollEvent(&event)){
			//ship controls
			myship.handle_input();

			//other controls
			if(event.type==SDL_KEYDOWN){
				switch( event.key.keysym.sym ){
					case SDLK_ESCAPE: quitGame=true; break;
					case SDLK_x:
						//BOMB HERE
						if(Mix_PlayChannel(-1,bomb,0)==-1) return 1; break;
				}
			}

			//if the window gets X'd
			if( event.type == SDL_QUIT ){
				quitGame = true;
			}
		}

		if(isCol(myship.hitbox,b[0].hitbox)) quitGame=true;
		if(isCol(myship.hitbox,b[1].hitbox)) quitGame=true;
		if(isCol(myship.hitbox,b[2].hitbox)) quitGame=true;
		if(isCol(myship.hitbox,b[3].hitbox)) quitGame=true;
		if(isCol(myship.hitbox,b[4].hitbox)) quitGame=true;
		if(b[0].hitbox.x>520) b[0].hitbox.x=120;
		if(b[0].hitbox.y>480) b[0].hitbox.y=0;
		if(b[1].hitbox.x>520) b[1].hitbox.x=120;
		if(b[1].hitbox.y>480) b[1].hitbox.y=0;
		if(b[2].hitbox.x>520) b[2].hitbox.x=120;
		if(b[2].hitbox.y>480) b[2].hitbox.y=0;
		if(b[3].hitbox.x>520) b[3].hitbox.x=120;
		if(b[3].hitbox.y>480) b[3].hitbox.y=0;
		if(b[4].hitbox.x>520) b[4].hitbox.x=120;
		if(b[4].hitbox.y>480) b[4].hitbox.y=0;

        //update screen data
        myship.move();							//update ship's position
        printb(0,0,background,screen);			//print background
//        printb(0,5,boss,screen);				//print boss
		myship.show();							//print position to screen

		b[0].xVel=3;
		b[0].yVel=1;
		b[0].hitbox.y+=b[0].yVel;
		b[0].hitbox.x+=b[0].xVel;
		printb(b[0].hitbox.x,b[0].hitbox.y,bullet,screen,NULL);

		b[1].xVel=1;
		b[1].yVel=5;
		b[1].hitbox.y+=b[1].yVel;
		b[1].hitbox.x+=b[1].xVel;
		printb(b[1].hitbox.x,b[1].hitbox.y,bullet,screen,NULL);

		b[2].xVel=4;
		b[2].yVel=2;
		b[2].hitbox.y+=b[2].yVel;
		b[2].hitbox.x+=b[2].xVel;
		printb(b[2].hitbox.x,b[2].hitbox.y,bullet,screen,NULL);

		b[3].xVel=6;
		b[3].yVel=1;
		b[3].hitbox.y+=b[3].yVel;
		b[3].hitbox.x+=b[3].xVel;
		printb(b[3].hitbox.x,b[3].hitbox.y,bullet,screen,NULL);

		b[4].xVel=10;
		b[4].yVel=2;
		b[4].hitbox.y+=b[4].yVel;
		b[4].hitbox.x+=b[4].xVel;
		printb(b[4].hitbox.x,b[4].hitbox.y,bullet,screen,NULL);

        std::stringstream time;
        time<<scoreT.getTicks()/250;
        points=TTF_RenderText_Shaded(menuFont,time.str().c_str(),pointsColor,{0,0,0});
        printb(10,15,points,screen);

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
