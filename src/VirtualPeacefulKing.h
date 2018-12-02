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

//Our first boss class
class VirtualPeacefulKing
{
public:
    //Constructor
    //This boss will have a value of shield which will protect him from direct attack. The boss's health will only decrease if the shield is gone
    //Shield will generate itself throughout the time
    //This boss will also have a special skill to generate blackhole in a given Time interval
    VirtualPeacefulKing(int initialHealth, SDL_Texture* sheet, int attack, int skillCD);

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
    void move(double deltax, double deltay, double step);

    //return x position
    double getXPosition();

    //return y position
    double getYPosition();

    //Get the rectangle from the sprite sheet
    SDL_Rect getRect();

    //Get the king's sprite sheet
    SDL_Texture* getSheet();

    //Get the camera of the king
    SDL_Rect getCamera();

private:

    //Some basic attributes of our king

    double ACCEL = 3600.0;
    double SPEED_LIMIT = 300.0;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    int hitPoints;
    int speed;
    int cd;
    double attackPower;

    attack* playerBlast;

    SDL_Texture* kingSheet;
    Physics phys;

    SDL_Rect kingCam;
    SDL_Rect kingRect;

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
