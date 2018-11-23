//generates music for game 

#include "INC_SDL.h"
#include "music.h"
#include "SDL_mixer.h"

bool play = true;

bool init();
bool loadMedia();
void close();

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gFire = NULL;

void music :: playMusic()
{
	play = check();

	if(play){

		Mix_PlayMusic( gMusic, -1 );

	}
}
 
void music :: fireSound()
{
	play = check();

	if(play){

		Mix_PlayChannel( -1, gFire, 0 );

	}
}

bool music :: check()
{
	if( !init() )
	{
		printf( "Failed to initialize sound!\n" );
		play = false;
	}
		
	if( !loadMedia() )
	{
		printf( "Failed to load media!\n" );
		play = false;
	}
}

bool music :: init()
{
	//Initialization flag
	success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}


bool music :: loadMedia()
{
	//Loading success flag
	success = true;

	//Load music
	gMusic = Mix_LoadMUS( "resources/sounds/music.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	//Load sound effects
	gFire = Mix_LoadWAV( "resources/sounds/fire.wav" );
	if( gFire == NULL )
	{
		printf( "Failed to load fire sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void music :: close()
{
	//Free the sound effects
	Mix_FreeChunk( gFire );
	gFire = NULL;
	
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Quit SDL subsystems
	Mix_Quit();
}
