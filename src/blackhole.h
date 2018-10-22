#ifndef blackhole_h
#define blackhole_h

#include <stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
class blackhole
{
public:
    blackhole(SDL_Renderer* Renderer, SDL_Texture* gBlackhole, SDL_Rect* blackholeRect,SDL_Rect blackholeCam);
    void showBlackhole(SDL_Rect blackholeCam);
private:
    SDL_Rect* blackholeRect;
    SDL_Renderer* gRenderer;
    SDL_Texture* gBlackhole;
};
#endif
