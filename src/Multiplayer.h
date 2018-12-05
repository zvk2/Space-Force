//Multiplayer header
#ifndef Multiplayer_H
#define Multiplayer_H

#include <stdio.h>
#include <stdlib.h>
#include "INC_SDL.h"
#include "Multiplayer.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "OpenGLRenderer.hpp"

class Multiplayer
{

public:

	Multiplayer(Player* p1, Player* p2, attack* atk1, attack* atk2);

	void PlayerCollision();

	void CheckPlayerCollision(Player* player2, double tstep);

	void TotalDamagePlayer1();

	void TotalDamagePlayer2();

	void AttackCollision1();

	void AttackCollision2();

	void DecrementHealthPlayer1(int decAmount1)

	void DecrementHealthPlayer2(int decAmount2);

    

private:
   
    
   SDL_Rect playerCam1;
   SDL_Rect playerCam2;

   double xCoord;
   double yCoord;

	int hitPoints1;
	int hitPoints2;



};
#endif