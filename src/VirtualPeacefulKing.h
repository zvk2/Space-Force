//
//  VirtualPeacefulKing.h
//  CS1666_SpaceForce
//
//  Created by XuZhisheng on 11/5/18.
//  Copyright © 2018 XuZhisheng. All rights reserved.
//

#ifndef KING_h
#define KING_h

#include "INC_SDL.h"
#include "physics.h"
#include "OpenGLRenderer.hpp"
#include "Player.h"
#include "attack.h"
#include <stdio.h>
#include <cmath>

//Our first boss class
class VirtualPeacefulKing
{
public:
    //Constructor
    //This boss will have a value of shield which will protect him from direct attack. The boss's health will only decrease if the shield is gone
    //Shield will generate itself throughout the time
    //This boss will also have a special skill to generate blackhole in a given Time interval
    VirtualPeacefulKing(OpenGLRenderer* gRend, int initialHealth, int attack, int skillCD, Player* main);

    //Damage to the shield
    //void LostShield(int damage);

    //Shield self recoveray
    //void RecoveryShield(int recoveryRate);

    //Damage to boss's health
    void LostHealth(int damage);

    //Animate our king
    void animate(int frames);

    //set the position of the king
    void setPosition(double x, double y);

    //Set the moving speed of the king
    void setVelocity(double x, double y);

    //a method which can be called from model class
    void move(double step);

    //return x position
    double getXPosition();

    //return y position
    double getYPosition();

    //Get the rectangle from the sprite sheet
    SDL_Rect getRect();

    //Get the king's sprite sheet
    //~ SDL_Texture* getSheet();

    //Get the camera of the king
    SDL_Rect getCamera();

    //Get the camera point of the king
    SDL_Rect* getCameraLoc();
    
    //To check whether the king and player have collision since both of them are rects
    bool checkRectCollision(SDL_Rect* a, SDL_Rect* b);

private:

    //Some basic attributes of our king
    OpenGLRenderer* openGL;
    RenderObject* render;
	Player* ply;

    double ACCEL = 3600.0;
    double SPEED_LIMIT = 300.0;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    int hitPoints;
    int speed;
    int cd;
    double attackPower;
	double delta;
    

    const char* kingTexture;

    //~ SDL_Texture* kingSheet;
    Physics phys;

    SDL_Rect kingCam;
    SDL_Rect kingRect;
	SDL_Rect* plyCam;

    double xCoord;
    double yCoord;


    //check the boundaries so that our king will not move outside the screen
    void checkBoundary();

    //To actually decrease the health
    void minusHealth(int amount);

    //To actually increase the health
    void plusHealth(int amount);
};//end of VirtualPeacefulKing class

//All the other potential future boss class
#endif /* KING_h */
