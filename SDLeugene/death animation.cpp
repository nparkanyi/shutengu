//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frames per second
const int FRAMES_PER_SECOND = 20;

//The dimenstions of the stick figure
const int SHIP_WIDTH = 40;
const int SHIP_HEIGHT = 50;
//The surfaces
SDL_Surface *ship = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//sprite sheet areas
SDL_Rect clips[5];

class Ship
{
	private:
	int offSet;
	int frame;
	int status;

	public:
	Ship();
	void handle_events();
	void show();
};//The timer
class Timer
{
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
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
void set_clips()
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
}
bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Animation Test", NULL );

    //If everything initialized fine
    return true;
}
bool load_files()
{
    //Load the sprite sheet
    ship = IMG_Load( "shipdeath.png" );

    //If there was a problem in loading the sprite
    if( ship == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( ship );

    //Quit SDL
    SDL_Quit();
}
Ship::Ship()
{
	//Initialize movement variables
	offSet=0;
    //Initialize animation variables
    frame = 0;
}
void Ship::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
    	if(event.key.keysym.sym==SDLK_x)
		{
			frame++;
			apply_surface(offSet,SCREEN_HEIGHT-SHIP_HEIGHT,ship,screen,&clips[frame]);
		}
    }
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
int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Clip the sprite sheet
    set_clips();

    //The frame rate regulator
    Timer fps;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

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
