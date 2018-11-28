//music header file
#ifndef MUSIC_H
#define MUSIC_H

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>

class music
{

	public:
		void playMusic();

		void fireSound();

		//~ bool check();

		bool init();

		bool loadMedia();

		void close();

	private:
		Mix_Music *gMusic;

		Mix_Chunk *gFire;

		bool success;

		bool play;

};

#endif
