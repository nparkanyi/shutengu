//Jon Liu, Eugene Wang, Ms. Odecki, ICS3U, January 2013
//DON'T RUN NETSUPPORT WHILE PLAYING THIS GAME!
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>

int iMaxBul=-1;       //how many bullets are currently allowed

//screen attributes
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;
const int FRAMES_PER_SECOND=60;		//a ratio to control rate of movement

//ship attributes
const int SHIP_WIDTH=20;
const int SHIP_HEIGHT=20;
const int SHIP_SPEED=200;

//wave timer
const int WAVE_LENGTH=30000;        //30 seconds

//surfaces starring in this production
SDL_Surface *sfMenu=NULL;				//menu screen
SDL_Surface *sfBG=NULL;					//game background screen
SDL_Surface *sfOverBG=NULL;				//game over screen
SDL_Surface *sfHowTo=NULL;				//how to play screen
SDL_Surface *sfShip=NULL;				//image of the ship
SDL_Surface *sfBullet=NULL;				//image of the bullet
SDL_Surface *sfBombsIcon=NULL;			//bomb icon
SDL_Surface *sfLivesIcon=NULL;			//lives icon
SDL_Surface *sfWavesIcon=NULL;			//waves so far icon
SDL_Surface *sfNewHigh=NULL;			//new highscore notification
SDL_Surface *sfDeathOverlay=NULL;		//died notification
SDL_Surface *sfBombFlash=NULL;			//bombed notification
SDL_Surface *sfScreen=NULL;				//the screen: everything prints to this

//text surfaces
SDL_Surface *sfMenuPrompt=NULL;			//text to prompt to start
SDL_Surface *sfRestart=NULL;			//text to prompt to restart
SDL_Surface *sfScore=NULL;				//text of the score
SDL_Surface *sfTime=NULL;				//text of the time remaining
SDL_Surface *sfBombs=NULL;				//text of the bombs remaining
SDL_Surface *sfLives=NULL;				//text of the lives remaining
SDL_Surface *surfWaves=NULL;            //ha ha ha surf wave
SDL_Surface *sfHighScore=NULL;			//text of the highscore

//the award-winning soundtrawhile(SDL_PollEvent(&event)){ck
Mix_Music *muBGM=NULL;					//persistent background music
Mix_Chunk *chDeath=NULL;				//death sound
Mix_Chunk *chGain=NULL;					//1-up sound
Mix_Chunk *chBomb=NULL;					//bomb sound

//fonts and colours
TTF_Font *fnMenu=NULL;					//font for menu prompt
TTF_Font *fnHUD=NULL;					//font for most of the game
TTF_Font *fnHighScore=NULL;				//font for the highscore
TTF_Font *fnFinalScore=NULL;			//font at game over
SDL_Color clDefault= {0,0,0};			//black
SDL_Color clMenu= {0xff,0xff,0xff};		//white
SDL_Color clScore= {0xff,0xff,0xff};	//white
SDL_Color clHighScore= {0xcc,0xcc,0xcc};//grey
SDL_Color clWaves= {0xaf,0xdd,0xe9};	//blue
SDL_Color clTime= {0xcc,0xff,0xaa};		//green
SDL_Color clBomb= {0xff,0xee,0xaa};		//yellow
SDL_Color clLives= {0xe9,0xaf,0xaf};	//red

//event handler
SDL_Event event;

//changeable game data
int iHighScore=0;       				//highscore variable
int iLife=3;           					//lives left
int iBomb=3;            				//bombs left
int iWave=0;            				//waves so far
int iScore=0;           				//score so far
int iScoreAccel=1;						//score gain rate
bool quitMenu=false;    				//maintains menu loop
bool quitGame=false;    				//maintains game loop
bool quitOver=false;    				//maintains game over loop
bool quitAll=false;	    			//allows player to replay
bool newHighScore=false;				//true if new high score
bool diedRecently=false;				//player has been hit
bool bombedRecently=false;				//player has used a bomb
bool waveZero=true;     				//true if first wave fo the game

//lists ship properties
class ship {
    public:
    	int xVel, yVel;                         //velocity
        SDL_Rect hitbox;						//collision space: nested strucure
        ship();                                 //initializes
        void handleInput();                    	//handles keyboard controls for the ship
        void move(Uint32 deltaTicks);         	//handles motion
        void show();                           //renders ship
};

struct bulletData {         					//bullet structure
    SDL_Rect hitbox;							//collision space: nested structure
    int xVel;									//velocity in X
    int yVel;									//velocity in Y
};

//A STRUCTURE ARRAY!
//it must be declared here in order for the bulletData data type to be valid
bulletData b[200];

//timer properties
class Timer {
    private:
        int startTicks;     	//time when it started
        int pausedTicks;    	//when paused: time past since start
        bool paused;       		//timer is on hold
        bool started;     		//timer is running

    public:
        Timer();          		//initializes
        void start();     		//start running
        void stop();			//stop running and set to zero
        int getTicks();     	//timer's recorded time
};

//renders surfaces in specified coordinate
void printb(int xpos,int ypos,SDL_Surface* orig,SDL_Surface* target,SDL_Rect* clip=NULL) {
    SDL_Rect offset;        //stores coordinates
    offset.x = xpos;
    offset.y = ypos;

    //render
    SDL_BlitSurface(orig,clip,target,&offset);
}

//initializes SDL subsystems
bool init() {
    //all SDL defaults or return false if error
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1) return false;

	//load program icon
	SDL_Surface *sfAppIcon=IMG_Load("img/appico.png");
	if(sfAppIcon==NULL) return false;
	SDL_WM_SetIcon(sfAppIcon,NULL);
	SDL_FreeSurface(sfAppIcon);

    //init screen or return false if error
    sfScreen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_FULLSCREEN);
    if(sfScreen==NULL) return false;

    //init SDL_mixer or return false if error
    if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1) return false;

    //init SDL_ttf or return false if error
    if(TTF_Init()==-1) return false;

    SDL_WM_SetCaption("Shutengu!!",NULL); 	//set window caption
    SDL_ShowCursor(SDL_DISABLE);            //hide mouse pointer

    //all is well if we reach this point
    return true;
}

//initializes most game assets
bool prepAssets() {
    int i;

    //load images or return false if error
    sfMenu=IMG_Load("img/menu.png");
    sfShip=IMG_Load("img/gameship.png");
    sfBG=IMG_Load("img/bg.png");
    sfBullet=IMG_Load("img/bullet.png");
    sfLivesIcon=IMG_Load("img/lives.png");
    sfBombsIcon=IMG_Load("img/bombs.png");
    sfWavesIcon=IMG_Load("img/waves.png");
    sfHowTo=IMG_Load("img/howto.png");
    sfOverBG=IMG_Load("img/gameover.png");
    sfNewHigh=IMG_Load("img/newhigh.png");
    sfDeathOverlay=IMG_Load("img/deathoverlay.png");
    sfBombFlash=IMG_Load("img/bombflash.png");
    if(sfMenu==NULL||
		sfShip==NULL||
		sfBG==NULL||
		sfBullet==NULL||
		sfLivesIcon==NULL||
		sfWavesIcon==NULL||
		sfHowTo==NULL||
		sfOverBG==NULL||
		sfNewHigh==NULL||
		sfDeathOverlay==NULL) return false;

    //load audio or return false if error
    muBGM=Mix_LoadMUS("audio/hahaha.wav");    //menu music by default
    chGain=Mix_LoadWAV("audio/1up.wav");
    chBomb=Mix_LoadWAV("audio/boom.wav");
    chDeath=Mix_LoadWAV("audio/death.wav");
    if(muBGM==NULL||
		chGain==NULL||
		chBomb==NULL||
		chDeath==NULL) return false;

    //open fonts or return false if error
    fnMenu=TTF_OpenFont("envy.ttf",24);
    fnHUD=TTF_OpenFont("envy.ttf",46);
    fnHighScore=TTF_OpenFont("envy.ttf",24);
    fnFinalScore=TTF_OpenFont("envy.ttf",64);
    if(fnMenu==NULL||
		fnHUD==NULL||
		fnHighScore==NULL||
		fnFinalScore==NULL) return false;

    //all is well
    return true;
}

//unloads assets and closes SDL subsystems
void cleanUp() {
    //free all surfaces
    SDL_FreeSurface(sfMenu);
    SDL_FreeSurface(sfShip);
    SDL_FreeSurface(sfBG);
    SDL_FreeSurface(sfMenuPrompt);
    SDL_FreeSurface(sfScreen);
    SDL_FreeSurface(sfBullet);
    SDL_FreeSurface(sfTime);
    SDL_FreeSurface(sfLives);
    SDL_FreeSurface(sfLivesIcon);
    SDL_FreeSurface(sfBombs);
    SDL_FreeSurface(sfBombsIcon);
    SDL_FreeSurface(surfWaves);
    SDL_FreeSurface(sfWavesIcon);
    SDL_FreeSurface(sfScore);
    SDL_FreeSurface(sfHowTo);
    SDL_FreeSurface(sfHighScore);
    SDL_FreeSurface(sfRestart);
    SDL_FreeSurface(sfOverBG);
    SDL_FreeSurface(sfNewHigh);
    SDL_FreeSurface(sfDeathOverlay);
    SDL_FreeSurface(sfBombFlash);

    //free all audio
    Mix_FreeMusic(muBGM);
    Mix_FreeChunk(chGain);
    Mix_FreeChunk(chBomb);
    Mix_FreeChunk(chDeath);
    Mix_CloseAudio();

    //close all fonts
    TTF_CloseFont(fnMenu);
    TTF_CloseFont(fnHUD);
    TTF_CloseFont(fnHighScore);
    TTF_CloseFont(fnFinalScore);
    TTF_Quit();

    //quit SDL
    SDL_Quit();
}

//randomly determines new music to play, then plays it
bool newBGM() {
    srand(time(NULL));
    int rMus=rand()%9+1;    //from 1 to 9

    //randomly picks new music form this pool
    switch(rMus) {
        case 1:
            muBGM=Mix_LoadMUS("audio/arps.wav");
            break;
        case 2:
            muBGM=Mix_LoadMUS("audio/chase1.wav");
            break;
        case 3:
            muBGM=Mix_LoadMUS("audio/chase2.wav");
            break;
        case 4:
            muBGM=Mix_LoadMUS("audio/gacd.wav");
            break;
        case 5:
            muBGM=Mix_LoadMUS("audio/train.wav");
            break;
        case 6:
            muBGM=Mix_LoadMUS("audio/hey.wav");
            break;
        case 7:
            muBGM=Mix_LoadMUS("audio/juice.wav");
            break;
        case 8:
            muBGM=Mix_LoadMUS("audio/partybell.wav");
            break;
        case 9:
            muBGM=Mix_LoadMUS("audio/pounds.wav");
            break;
		default:;
    }

    //play it or return false if error
    if(Mix_PlayMusic(muBGM,-1)==-1) return false;

    //all is well
    return true;
}

//gives coordinates to all new bullets
void prepBullets() {
    int i;
    for(i=0; i<=iMaxBul; i++) {
        b[iMaxBul].hitbox.x=rand()%420+120;
        b[iMaxBul].hitbox.h=5;
        b[iMaxBul].hitbox.w=5;
        b[iMaxBul].hitbox.y=0;
    }
}

//change spawning bullet's position
void randBullets() {
    int i;
    for(i=0; i<iMaxBul; i++) {
        b[i].xVel=rand()%4+1;
        b[i].yVel=rand()%4+1;
    }
}

//player levels up
void nextWave() {
    iMaxBul+=2;                    		//increase bullet total

    //new bullet velocities
    b[iMaxBul].xVel=rand()%5-2;
    b[iMaxBul].yVel=rand()%4+1;
    b[iMaxBul-1].xVel=rand()%5-2;
    b[iMaxBul-1].yVel=rand()%4+1;

    //make new bullets
    prepBullets();
}

//checks for collision
bool isCol(SDL_Rect rectA,SDL_Rect rectB) {
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
    if(bottomA<=topB) return false;
    if(topA>=bottomB) return false;
    if(rightA<=leftB) return false;
    if(leftA>=rightB) return false;

    //If none of the sides from A are outside B
    return true;
}

//initializes ship's properties
ship::ship() {
    hitbox.x=(SCREEN_WIDTH-SHIP_WIDTH)/2;			//start at centre
    hitbox.y=400;									//near the bottom
    hitbox.h=SHIP_HEIGHT;
    hitbox.w=SHIP_WIDTH;
    xVel = 0;       								//ship's initial velocity
    yVel = 0;       								//of course it's zero
}

//responds to keypresses involving the ship
//73 is the magic number that prevents ship moving bugs
//the game prefers the ship's top left corner for some reason
void ship::handleInput() {
    //key is pressed
    if(event.type==SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_UP:
                yVel-=SHIP_SPEED-73;
                break;
            case SDLK_DOWN:
                yVel+=SHIP_SPEED;
                break;
            case SDLK_LEFT:
                xVel-=SHIP_SPEED-73;
                break;
            case SDLK_RIGHT:
                xVel+=SHIP_SPEED;
                break;
			default:;
        }
    }

    //key is released
    if(event.type==SDL_KEYUP) {
        switch(event.key.keysym.sym) {
            case SDLK_UP:
                yVel +=SHIP_SPEED-73;
                break;
            case SDLK_DOWN:
                yVel -=SHIP_SPEED;
                break;
            case SDLK_LEFT:
                xVel +=SHIP_SPEED-73;
                break;
            case SDLK_RIGHT:
                xVel -=SHIP_SPEED;
                break;
			default:;
        }
    }
}

//controls ship movement
//ship moves based on time passed, not by frames rendered
void ship::move(Uint32 DeltaTicks) {
    //horizontal motion: stop the ship if out of bounds
    hitbox.x+=xVel*(DeltaTicks/1000.f);
    if(hitbox.x<120) hitbox.x=120;
    else if(hitbox.x+SHIP_WIDTH>520) hitbox.x=500;

    //vertical motion: stop the ship if out of bounds
    hitbox.y+=yVel*(DeltaTicks/1000.f);
    if(hitbox.y<0) hitbox.y=0;
    else if(hitbox.y+SHIP_HEIGHT>480) hitbox.y=460;
}

//renders the ship
void ship::show() {
    printb(hitbox.x, hitbox.y, sfShip, sfScreen);
}

//init timer properties
Timer::Timer() {
    startTicks=0;
    pausedTicks=0;
    paused=false;
    started=false;
}

//starts the timer
void Timer::start() {
    started=true;             //it is now running
    paused=false;             //it is not paused
    startTicks=SDL_GetTicks();//time when timer was started
}
void Timer::stop(){
    started=false;			//Stop the timer
    paused=false;			//Unpause the timer
}

//finds time since timer start
int Timer::getTicks() {
    //run only if timer is active
    if(started==true) {
        if(paused==true) return pausedTicks;
        else return(SDL_GetTicks()-startTicks);
    }

    //at this point: timer isn't running
    return 0;
}


ship myship;            //user-controlled ship

//timers
Timer tmFPS;            //controls frame rate
Timer tmFPSUpd;         //total fps updates
Timer tmDelta;          //track the change in time
Timer tmDeathOverlay;	//how long to display death notification
Timer tmBombFlash;		//how long to display bomb notification
Timer tmTimeAlive;		//how long player has been alive
Timer tmTime;           //time until next wave
Timer tmScore;          //frequency of score increase
Timer tmMusic;			//when to change music

//reset game variables to ensure a clean replay
void resetGame(){
	tmTime.start();						//restart wave timer
	tmScore.stop();						//stop counting score

	//reset stats
	iLife=3;
	iBomb=3;
	iWave=0;
	iScore=0;
	iMaxBul=-1;

	//reset booleans
	waveZero=true;
	quitGame=false;
	quitOver=false;
	newHighScore=false;

	//reset ship properties
	myship.xVel=0;
	myship.yVel=0;
	myship.hitbox.x=(SCREEN_WIDTH-SHIP_WIDTH)/2;
    myship.hitbox.y=400;

    if(tmScore.getTicks()>250){
		iScore++;
		tmScore.start();
	}

}

//display game stats
void renderHUD() {
    //display the score
    std::stringstream sScore;
    sScore<<iScore;
    sfScore=TTF_RenderText_Shaded(fnHUD,sScore.str().c_str(),clScore,clDefault);
    printb(7,0,sfScore,sfScreen);

    //display the wave number
    std::stringstream sWaves;
    sWaves<<iWave;
    surfWaves=TTF_RenderText_Shaded(fnHUD,sWaves.str().c_str(),clWaves,clDefault);
    printb(568,-1,surfWaves,sfScreen);
    printb(530,10,sfWavesIcon,sfScreen);

    //display the time remaining in the wave
    std::stringstream sTime;
    if(waveZero==true) sTime<<10000/100-tmTime.getTicks()/100;
    else sTime<<WAVE_LENGTH/100-tmTime.getTicks()/100;
    sfTime=TTF_RenderText_Shaded(fnHUD,sTime.str().c_str(),clTime,clDefault);
    printb(7,425,sfTime,sfScreen);

    //displaying bombs left
    std::stringstream sBombs;
    sBombs<<iBomb;
    sfBombs=TTF_RenderText_Shaded(fnHUD,sBombs.str().c_str(),clBomb,clDefault);
    printb(568,380,sfBombs,sfScreen);
    printb(530,391,sfBombsIcon,sfScreen);

    //displaying lives left
    std::stringstream sLives;
    sLives<<iLife-1;
    sfLives=TTF_RenderText_Shaded(fnHUD,sLives.str().c_str(),clLives,clDefault);
    printb(568,427,sfLives,sfScreen);
    printb(530,438,sfLivesIcon,sfScreen);
}

//the bomb: only works if enough bombs
bool useBomb(){
	int i;

	if(iBomb>0) {
		iBomb--;

		//reset all bullets
		for(i=0; i<=iMaxBul; i++) {
			b[i].hitbox.y=-480;
			b[i].xVel=rand()%5-2;
			b[i].yVel=rand()%4+1;
		}

		//score penalty
		iScore-=25;

		//relevant stat tracking
		bombedRecently=true;
        tmBombFlash.start();
        tmTimeAlive.start();

        //play sound
		if(Mix_PlayChannel(-1,chBomb,0)==-1) return false;
	}
	return true;
}

//finally here. Whew that's a lot of functions
int main(int argc,char* args[]) {
    srand(time(NULL));  	//spin the wheel!
    int i;                  //loop counter
	int frame=0;            //total frames past

    if(init()==false) return 1;
    if(prepAssets()==false) return 1;
    if(Mix_PlayMusic(muBGM,-1)==-1) return 1;

    //read and store the string of appropriate language
    FILE *pLang;
    char strLang[25];       //language string
    if((pLang=fopen("text/en.WhyCantIHoldAllTheseFileExtensions","r"))!=NULL) {
        if(fgets(strLang,25,pLang)==NULL) return 1;
    }
    fclose(pLang);
    sfMenuPrompt=TTF_RenderText_Blended(fnMenu,strLang,clMenu);

	//read and store current highscore
	FILE *pHighScoreR;
	char strHighScore[10];
	if((pHighScoreR=fopen("text/highscore.WhyCantIHoldAllTheseFileExtensions","r"))!=NULL){
		if(fgets(strHighScore,10,pHighScoreR)==NULL) return 1;
	}
	fclose(pHighScoreR);
	sfHighScore=TTF_RenderText_Shaded(fnHighScore,strHighScore,clHighScore,clDefault);
	iHighScore=atoi(strHighScore);		//string contents as an int

	//menu runs here
	while(quitMenu==false) {
		//display menu
		printb(0,0,sfMenu,sfScreen);
		printb((SCREEN_WIDTH-sfMenuPrompt->w)/2,315,sfMenuPrompt,sfScreen);
		//menu-only key controls
		while(SDL_PollEvent(&event)) {
			if(event.type==SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_RETURN:
						quitMenu=true;
						break;
					case SDLK_ESCAPE:
						quitMenu=true;
						quitGame=true;
						quitOver=true;
						quitAll=true;
						break;
					default:;
				}
			}

			//if the window gets X'd
			if(event.type==SDL_QUIT) {
				quitMenu=true;          //quit the menu
				quitGame=true;          //skip the game
				quitOver=true;
				quitAll=true;
			}
		}

		//refresh the screen
		if(SDL_Flip(sfScreen)==-1) return 1;
	}

	//game is starting! set up everything!
	tmTime.start();
	tmFPS.start();
	tmFPSUpd.start();
	tmDelta.start();
	tmMusic.start();

	//REPLAY LOOP
	while(quitAll==false){
		randBullets();
		//game runs here
		while(quitGame==false) {
			//once wave time is up: level up and restart wave timer
			//setup phase is active
			if(waveZero==true&&tmTime.getTicks()>10000) {
				iWave++;
				nextWave();
				tmTime.start();
				waveZero=false;
				newBGM();
				tmScore.start();
				tmTimeAlive.start();
			}
			//setup phase is off
			else if(tmTime.getTicks()>WAVE_LENGTH) {
				iWave++;
				nextWave();
				tmTime.start();
			}

			//change music after 90 seconds
			if(tmMusic.getTicks()>90000){
				newBGM();
				tmMusic.start();
			}

			//score acceleration
			if(tmTimeAlive.getTicks()>30000) iScoreAccel=13;
			else if(tmTimeAlive.getTicks()>15000) iScoreAccel=6;
			else if(tmTimeAlive.getTicks()>7500) iScoreAccel=3;
			else if(tmTimeAlive.getTicks()>0) iScoreAccel=1;

			//score timing
			if(tmScore.getTicks()>250){
				iScore+=iScoreAccel;
				tmScore.start();
			}

			//1up timing
			if(tmTimeAlive.getTicks()>45000) {
				tmTimeAlive.start();
				iLife++;

				//don't let player have too many lives
				//if 1up is allowed, play sound
				if(iLife>5)iLife=5;
				else if(Mix_PlayChannel(-1,chGain,0)==-1) return 1;
			}

			//while there's science to do
			while(SDL_PollEvent(&event)) {
				//ship controls
				myship.handleInput();

				//other controls
				if(event.type==SDL_KEYDOWN) {
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							quitGame=true;
							quitOver=true;
							quitAll=true;
							break;
						case SDLK_x:
							if(useBomb()==false) return 1;
							break;
						default:;
					}
				}

				//if the window gets X'd
				if(event.type == SDL_QUIT) {
					quitGame = true;
					quitOver=true;
					quitAll=true;
				}
			}


			//update screen data
			myship.move(tmDelta.getTicks());    //update ship's position
			tmDelta.start();                    //restart change of time timer
			printb(0,0,sfBG,sfScreen);          //print background
			myship.show();                      //print position to screen
			if(diedRecently==true) printb(120,0,sfDeathOverlay,sfScreen,NULL);
			if(bombedRecently==true) printb(120,0,sfBombFlash,sfScreen,NULL);

			if(waveZero==true){					//reset bullets to original when looping game
				printb(0,0,sfHowTo,sfScreen,NULL);
				iMaxBul=-1;
			}
            for(i=0; i<=iMaxBul; i++) {
				//player has died: do all relevant tracking
                if(isCol(myship.hitbox,b[i].hitbox)) {
                    iLife--;
                    iBomb=3;
                    iScore-=50;
                    if(iLife==0) quitGame=true;
                    diedRecently=true;
                    b[i].hitbox.x=rand()%420-120;
                    b[i].hitbox.y=0;
                    tmDeathOverlay.start();
                    tmTimeAlive.start();
                    if(Mix_PlayChannel(-1,chDeath,0)==-1) return 1;
                }

                if(b[i].hitbox.x>515) b[i].hitbox.x=120;
                if(b[i].hitbox.x<120) b[i].hitbox.x=515;        //compensate for bullet width
                if(b[i].hitbox.y>480) {                         //because collision is counted from sScore of the picture
                    b[i].hitbox.y=0;                            //so bulletwidth had to be subtracted
                    b[i].xVel=rand()%5-2;                      //bullet can travel left or right
                    b[i].yVel=rand()%4+1;                       //can only travel down
                }
                b[i].hitbox.y+=b[i].yVel;
                b[i].hitbox.x+=b[i].xVel;
                printb(b[i].hitbox.x,b[i].hitbox.y,sfBullet,sfScreen,NULL);
            }

			//expiry dates for death and bomb notifications
            if(tmDeathOverlay.getTicks()>500) diedRecently=false;
            if(tmBombFlash.getTicks()>250) bombedRecently=false;

			//display all stats
			renderHUD();
			printb(7,50,sfHighScore,sfScreen,NULL);

			//refresh the screen
			if(SDL_Flip(sfScreen)==-1) return 1;

			//limit the frame rate
			if(tmFPS.getTicks()<1000/FRAMES_PER_SECOND) {
				SDL_Delay((1000/FRAMES_PER_SECOND)-tmFPS.getTicks());
				tmFPS.start();
			}

			frame++;    //one frame has passed

			//update this once per second
			if(tmFPSUpd.getTicks()>1000) {
				std::stringstream newCaption;
				newCaption<<frame/(tmFPS.getTicks()/1000.f)<<" fps";
				SDL_WM_SetCaption(newCaption.str().c_str(),NULL);
				tmFPSUpd.start();         //restart for the next one-second wait
			}
		}

		//store new high score, if there is one
		if(iScore>iHighScore){
			FILE *pHighScoreW;
			if((pHighScoreW=fopen("text/highscore.WhyCantIHoldAllTheseFileExtensions","w"))!=NULL){
				if(fprintf(pHighScoreW,"%d",iScore)==0) return 1;
			}
			fclose(pHighScoreW);
			newHighScore=true;
		}

		//stop playing music
		Mix_HaltMusic();

		//game over runs here
		while(quitOver==false) {
			//some key events
			while(SDL_PollEvent(&event)) {
				if(event.type==SDL_KEYDOWN) {
					switch(event.key.keysym.sym) {
						case SDLK_RETURN: quitOver=true; break;
						case SDLK_ESCAPE:
							quitOver=true;
							quitAll=true;
							break;
						default:;
					}
				}

				//if the window gets X'd
				if(event.type == SDL_QUIT){
					quitOver=true;
					quitAll=true;
				}
			}

			//end surfaces
			std::stringstream finalScore;
			finalScore<<iScore;
			sfScore=TTF_RenderText_Blended(fnFinalScore,finalScore.str().c_str(),clMenu);

			//display restart prompt
			FILE *pRestart;
			char strRestart[30];
			if((pRestart=fopen("text/enr.WhyCantIHoldAllTheseFileExtensions","r"))!=NULL){
				if(fgets(strRestart,30,pRestart)==NULL) return 1;
			}
			fclose(pRestart);
			sfRestart=TTF_RenderText_Blended(fnMenu,strRestart,clScore);

			//print everything
			printb(0,0,sfOverBG,sfScreen,NULL);
			printb((SCREEN_WIDTH-sfRestart->w)/2,385,sfRestart,sfScreen,NULL);
			printb((SCREEN_WIDTH-sfScore->w)/2,240,sfScore,sfScreen,NULL);
			if(newHighScore==true)	printb(430,280,sfNewHigh,sfScreen,NULL);

			//refresh the screen
			if(SDL_Flip(sfScreen)==-1) return 1;

			SDL_WM_SetCaption("Shutengu!!",NULL);
		}

		//reset loop conditions to allow replaying
		resetGame();
	}
    //user has now quit
    cleanUp();

    return 0;
}
