#include "blackhole.h"
#include<stdlib.h>
#include "INC_SDL.h"
#include "Player.h"
#include <math.h>

#define PI 3.14159265


blackhole::blackhole(SDL_Texture* textBlackhole, Player* main): ply(main), gBlackhole(textBlackhole)
        {
            gRenderer = ply->getRend();
            playerCam = ply->getPlayerCamLoc();
            blackholeRect = {0, 0, 300, 300};
            blackholeCam = {1680, 720/2, 300, 300};
            blackholeExists = false;
            bFrames = 0;
            gravAccel = 1;
        }

        bool blackhole::seen()
        {
            return blackholeExists;
        }

        void blackhole::showBlackhole()
        {
            bFrames++;
            
            if (bFrames / 12 > 5)
            {
                bFrames = 0;
            }
            blackholeRect.x = (bFrames / 12) * 300;

            if(!blackholeExists)
            {
                blackholeExists = true;
                blackholeCam.y = rand() % (720-300);
                blackholeCam.x = 1580;
            }
            
            attractPlayer();
            SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
            blackholeCam.x = blackholeCam.x - 1;
            
            if(blackholeCam.x < -300)
            {
                blackholeExists = false;
            }
            
        }

        void blackhole::attractPlayer()
        {
            double bx = blackholeCam.x;
            double by = blackholeCam.y;
            double px = playerCam->x;
            double py = playerCam->y;
            double hVecX;
            double hVecY;
            double hVecZ;
            double angleH;
            double vVecX;
            double vVecY;
            double newX;
            double newY;
            
            
            if(blackholeCam.x < 1680 && blackholeCam.x > 0)
            {
                //player to the left
                if(blackholeCam.x + 150 > playerCam->x)
                {
                    //player above
                    if(blackholeCam.y + 150 > playerCam->y)
                    {
                        hVecX = abs(bx - px);
                        hVecY = abs(by - py);
                        
                        hVecZ = sqrt(pow(hVecX,2) + pow(hVecY,2));
                        
                        angleH = acos(hVecX/hVecZ) * 180.0 / PI;
                        
                        vVecX = tan(angleH*PI/180) * abs(hVecY);
                        
                        vVecY = hVecY;
                        
                        newX = (abs(hVecX) + abs(vVecX)) / 2;
                        newY = (abs(hVecY) + (-vVecY)) / 2;
                        
                        gravAccel++;
                        
                        newX = newX + pow(gravAccel, 2);
                        newY = newY / 5;
                    }
                    //player below
                    if(blackholeCam.y + 150 < playerCam->y)
                    {
                        hVecX = abs(bx - px);
                        hVecY = abs(by - py);
                        
                        hVecZ = sqrt(pow(hVecX,2) + pow(hVecY,2));
                        
                        angleH = acos(hVecX/hVecZ) * 180.0 / PI;
                        
                        vVecX = tan(angleH*PI/180) * abs(hVecY);
                        
                        vVecY = hVecY;
                        
                        newX = (abs(hVecX) + (-vVecX)) / 2;
                        newY = ((-hVecY) + (-vVecY)) / 2;
                        
                        gravAccel++;
                        
                        newX = newX + pow(gravAccel, 2);
                        newY = newY / 5;
                    }
                    
                }
                //player to the right
                else if(blackholeCam.x + 150 < playerCam->x)
                {
                    //player above
                    if(blackholeCam.y + 150 > playerCam->y)
                    {
                        hVecX = abs(bx - px);
                        hVecY = abs(by - py);
                        
                        hVecZ = sqrt(pow(hVecX,2) + pow(hVecY,2));
                        
                        angleH = acos(hVecX/hVecZ) * 180.0 / PI;
                        
                        vVecX = tan(angleH*PI/180) * abs(hVecY);
                        
                        vVecY = hVecY;
                        
                        newX = ((-hVecX) + abs(vVecX)) / 2;
                        newY = (abs(hVecY) + abs(vVecY)) / 2;
                        
                        gravAccel++;
                        
                        newX = newX + pow(gravAccel, 2);
                        newY = newY / 5;
                    }
                    //player below
                    if(blackholeCam.y + 150 < playerCam->y)
                    {
                        hVecX = abs(bx - px);
                        hVecY = abs(by - py);
                        
                        hVecZ = sqrt(pow(hVecX,2) + pow(hVecY,2));
                        
                        angleH = acos(hVecX/hVecZ) * 180.0 / PI;
                        
                        vVecX = tan(angleH*PI/180) * abs(hVecY);
                        
                        vVecY = hVecY;
                        
                        newX = ((-hVecX) + (-vVecX)) / 2;
                        newY = ((-hVecY) + abs(vVecY)) / 2;
                        
                        gravAccel++;
                        
                        newX = newX + pow(gravAccel, 2);
                        newY = newY / 5;
                    }
                    
                }
            }
            
            
        }

