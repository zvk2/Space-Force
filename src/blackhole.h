#ifndef blackhole_h
#define blackhole_h

#include <stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
#include "Player.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

class blackhole
{
public:
    blackhole(SDL_Texture* textBlackhole, Player* main);
    void showBlackhole(double xDeltav, double yDeltav, double timestep);
    bool seen();
    void attractPlayer(double xDeltav, double yDeltav, double timestep);
private:
    SDL_Rect blackholeRect;
    SDL_Renderer* gRenderer;
    SDL_Texture* gBlackhole;
    Player* ply;
    SDL_Rect blackholeCam;
    bool blackholeExists;
    int bFrames;
    SDL_Rect* playerCam;
    int gravAccel;
    double xDeltav;
    double yDeltav;
    double timestep;
};
#endif