//generates music for game

#include "INC_SDL.h"
#include "music.h"
#include "SDL_mixer.h"





//The music that will be played


//The sound effects that will be used


void music::playMusic()
{
	if (success) {
		Mix_PlayMusic(gMusic, -1);
	}
}

void music::fireSound()
{
	if (success) {
		Mix_PlayChannel(1, gFire, 0);
	}
}

// A little strange to have this in lieu of a constructor
bool music::init()
{
	gMusic = nullptr;
	gFire = nullptr;
	//Initialization flag
	success = true;

	// Please do not double initialize
	//Initialize SDL
	//~ if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	//~ {
		//~ printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		//~ success = false;
	//~ }


	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	success = loadMedia();

	return success;
}


bool music::loadMedia()
{
	//Loading success flag
	success = true;

	//Load music
	gMusic = Mix_LoadMUS("resources/sounds/spaceJourneyDangerous.wav");
	if(gMusic == NULL)
	{
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	//Load sound effects
	gFire = Mix_LoadWAV("resources/sounds/fire.wav");
	if(gFire == NULL)
	{
		printf("Failed to load fire sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

void music::close()
{
	//Free the sound effects
	Mix_FreeChunk(gFire);
	gFire = nullptr;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = nullptr;

	//Quit SDL subsystems
	Mix_Quit();
}
