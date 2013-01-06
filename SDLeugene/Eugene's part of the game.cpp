//Jon Lui, Eugene Wang, Ms. Odecki, ICS 3U, 2012

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
#include "SDL/SDL_mixer.h"
=======
#include "SDL/SDL_ttf.h"
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
#include <string>
#include <stdio.h>

//The screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
int movespeed=3;

//The frame rate
const int FRAMES_PER_SECOND = 60;

//The dimensions of the ship
const int SHIP_WIDTH = 40;
const int SHIP_HEIGHT = 50;

//The surfaces
SDL_Surface *menu = NULL;
SDL_Surface *theship =NULL;
SDL_Surface *shipdeath = NULL;
SDL_Surface *background = NULL;
SDL_Surface *boss = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *menumsg= NULL;

//The audio
Mix_Music *bgm=NULL;
Mix_Chunk *gain=NULL;

//The event structure
SDL_Event event;

//font and color
TTF_Font *font = NULL;
SDL_Color textColor = {255,0,255};

//clips
SDL_Rect clips[5];
//The ship that will move around on the screen

class ship
{
    private:
    //The X and Y offsets
    int x, y;

    //The velocity of the dot
    int xVel, yVel;

	int frame;

    public:
    //Initializes the variables
    ship();

    //Takes key presses and adjusts the velocity
    void handle_input();

    //Moves the dot
    void move();

    //Shows the ship on the screen
    void show();
};

//The timer
class Timer{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};
void printb( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ){
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp

bool init(){
=======
/*void set_clips()
{
	//clip sprites
	clips[0].x=0;
	clips[0].y=0;
	clips[0].w=SHIP_WIDTH;
	clips[0].h=SHIP_HEIGHT;
	clips[1].x=0;
	clips[1].y=0;
	clips[1].w=SHIP_WIDTH;
	clips[1].h=SHIP_HEIGHT;
	clips[2].x=SHIP_WIDTH*2;
	clips[2].y=0;
	clips[2].w=SHIP_WIDTH;
	clips[2].h=SHIP_HEIGHT;
	clips[3].x=SHIP_WIDTH*3;
	clips[3].y=0;
	clips[3].w=SHIP_WIDTH;
	clips[3].h=SHIP_HEIGHT;
	clips[4].x=SHIP_WIDTH*4;
	clips[4].y=0;
	clips[4].w=SHIP_WIDTH;
	clips[4].h=SHIP_HEIGHT;
}*/
bool init()
{
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
    //Start SDL_mixer
    if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1){
		return false;
    }

=======
	//initialize ttf
	if (TTF_Init()==-1){
		return false;
	}
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
    //Set the window caption
    SDL_WM_SetCaption( "Prototype", NULL );

    //If everything initialized fine
    return true;
}

<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
bool prepAssets(){
    //load images
=======
bool load_files()
{
    //Load the Ship image
    menu = IMG_Load("menu.png");
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
    theship = IMG_Load( "gameship.png" );
    shipdeath = IMG_Load("shipdeath.png");
    background = IMG_Load("background.png");
	boss = IMG_Load("boss.png");

	//open font
	font = TTF_OpenFont("lazy.ttf",28);

    //If there was a problem in loading the ship
    if(( theship == NULL) || (background==NULL) || (boss==NULL) || (shipdeath==NULL) || (menu==NULL) || (font==NULL) )
    {
        return false;
    }

	//load audio
	bgm=Mix_LoadWAV("gacd.wav");

	if((bgm==NULL)){
		return false;
	}

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface(theship);
    SDL_FreeSurface(shipdeath);
    SDL_FreeSurface(background);
    SDL_FreeSurface(boss);
    SDL_FreeSurface(menu);
	SDL_FreeSurface(menumsg);

	//Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

ship::ship()
{
    //Initialize the offsets
    x = 300;
    y = 400;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void ship::handle_input()
{
    //If a key was pressed, prepare to do something
    if( event.type == SDL_KEYDOWN )
    {
    	//check which key is pressed
		switch( event.key.keysym.sym )
		{
<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
//			case SDLK_LSHIFT: movespeed=1; break;			//does not work properly
=======
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
			case SDLK_UP: yVel -=movespeed; break;
			case SDLK_DOWN: yVel +=movespeed; break;
			case SDLK_LEFT: xVel -=movespeed; break;
			case SDLK_RIGHT: xVel +=movespeed; break;
		}
	}
    //If a key was released, prepare to do something
	if( event.type == SDL_KEYUP )
    {
    	//check which key is released
		switch( event.key.keysym.sym )
		{
<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
//			case SDLK_LSHIFT: movespeed=10; break;			//does not work properly
=======
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
			case SDLK_UP: yVel +=movespeed; break;
			case SDLK_DOWN: yVel -=movespeed; break;
			case SDLK_LEFT: xVel +=movespeed; break;
			case SDLK_RIGHT: xVel -=movespeed; break;
		}
    }
}

void ship::move()
{
    //Move the ship left or right
    x += xVel;

    //If the ship went too far to the left or right
    if( ( x < 170 ) || ( x + SHIP_WIDTH > 454 ) )
    {
        //move back
        x -= xVel;
    }

    //Move the ship up or down
    y += yVel;

    //If the ship went too far up or down
    if( ( y < 0 ) || ( y + SHIP_HEIGHT > SCREEN_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }
}

void ship::show()
{
    //Show the Ship
<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
    printb( x, y, theship, screen );
=======
    apply_surface( x, y, theship, screen );
    /*//If a key was pressed, prepare to do something
    if( event.type == SDL_KEYDOWN )
    {
    	//check which key is pressed
		if( event.key.keysym.sym==SDLK_x )
			frame++;
    }
    apply_surface(x,y,shipdeath,screen,&clips[frame]);*/
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

int main(int argc, char* args[])
{

	FILE *fptr;
	char msg[100];
	if ((fptr=fopen("swkey.txt","r"))!=NULL){
		if (fgets(msg,100,fptr)==NULL){
			printf("error");
		}
		fclose(fptr);
	}


    //Quit flag
    bool quit = false;

    //The dot that will be used
    ship myship;

    //The frame rate regulator
    Timer fps;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( prepAssets() == false )
    {
        return 1;
    }

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the dot
            myship.handle_input();

            //If the user has Xed out the window
            if(( event.type == SDL_QUIT )||(event.key.keysym.sym==SDLK_ESCAPE))
            {
                //Quit the program
                quit = true;
            }
        }

        //Move the ship
        myship.move();

<<<<<<< HEAD:SDLeugene/prototype of game (motion).cpp
        //Show the ship/bg on the screen
        printb(0,0,background,screen);
        myship.show();
=======
        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		if (event.type==SDL_KEYDOWN)
		{
			menu=NULL;
		}
        //Show the ship/bg on the screen
        menumsg = TTF_RenderText_Solid(font, msg, textColor);
		apply_surface(0,0,background,screen);
		apply_surface(0,0,boss,screen);
		myship.show();
		apply_surface(0,0,menu,screen);
		apply_surface(20,400, menumsg,menu);
>>>>>>> 6ad503b240b3c7eda393f79adca5d9ce4a3c79f7:SDLeugene/Eugene's part of the game.cpp

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

    }

    //Clean up
    clean_up();

    return 0;
}
