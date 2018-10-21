#ifndef blackhole_h
#define blackhole_h

#include <stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
class blackhole
{
public:
    void spawnBlackhole(SDL_Renderer* Renderer, SDL_Texture* gBlackhole, SDL_Rect* rectBlackhole,SDL_Rect cam);
private:
    SDL_Rect* attackBox;
    SDL_Renderer* gRenderer;
    SDL_Texture* gAttack;
};
#endif
