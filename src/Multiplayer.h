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

	Multiplayer();

	void FlipPlayer2();

	void DamagePlayer1();

	void DamagePlayer2();

	void AttackCollision1();

	void AttackCollision1();

	void AttackCollision2();

    

private:
   

};
#endif