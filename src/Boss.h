//
//  Boss.h
//  CS1666_SpaceForce
//
//  Created by XuZhisheng on 11/5/18.
//  Copyright © 2018 XuZhisheng. All rights reserved.
//

#ifndef BOSS_h
#define BOSS_h
#endif /* BOSS_h */

#include "INC_SDL.h"
#include "physics.hpp"

//Our first boss class
class VirtualPeacefulKing
{
public:
    //Constructor
    //This boss will have a value of shield which will protect him from direct attack. The boss's health will only decrease if the shield is gone
    //Shield will generate itself throughout the time
    //This boss will also have a special skill to generate blackhole in a given Time interval
    VirtualPeacefulKing(int initialHealth, SDL_Texture* sheet, int attack, int shieldHealth, int skillCD);
    
    //Damage to the shield
    void LostShield(int damage);
    
    //Shield self recoveray
    void RecoveryShield(int recoveryRate);
    
    //Damage to boss's health
    void LostHealth(int damage);
    
private:
    
    
};//end of VirtualPeacefulKing class



//All the other potential future boss class
