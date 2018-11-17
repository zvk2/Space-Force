#ifndef blackhole_h
#define blackhole_h

#include <stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
#include "Player.h"

class blackhole
{
public:
    blackhole(SDL_Texture* textBlackhole, Player* main);
    void showBlackhole();
private:
    SDL_Rect blackholeRect;
    SDL_Renderer* gRenderer;
    SDL_Texture* gBlackhole;
    Player* ply;
    SDL_Rect blackholeCam;
    bool blackholeExists;
    int bFrames;
};
#endif
