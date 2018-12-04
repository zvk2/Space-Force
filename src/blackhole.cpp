#include "blackhole.h"
#include<stdlib.h>
#include "INC_SDL.h"
#include "Player.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#define PI 3.14159265
// NOTE AT THE MOMENT BLACKHOLE JUST KEEPS SCROLLING
		blackhole::blackhole(Player* main): ply(main)
		{
			//~ char blackHoleTexture[] = "resources/imgs/blackhole.png";

			openGL = ply->getRend();
			playerCam = ply->getPlayerCamLoc();
			blackholeRect = {0, 0, 300, 300};
			blackholeCam = {1680, 720/2, 300, 300};
			blackholeExists = false;
			bFrames = 0;
			gravAccel = 1;

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

        void blackhole::showBlackhole(double xDeltav, double yDeltav, double timestep)
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
            
            attractPlayer(xDeltav, yDeltav, timestep);
            SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
            blackholeCam.x = blackholeCam.x - 1;
            
            if(blackholeCam.x < -300)
            {
                blackholeExists = false;
            }
            
        }

        void blackhole::attractPlayer(double xDeltav, double yDeltav, double timestep)
        {
            double bx = blackholeCam.x;
            double by = blackholeCam.y;
            double px = playerCam->x;
            double py = playerCam->y;
            double x1;
            double y1;
            double r;
            double angleH;
			double angleN;
			double z1;
			double Fx;
			double Fy;
			double z2;
            double Vx;
            double Vy;
            double newX;
            double newY;
			double GM = 5000000;
			double m = 10;
			double maxSpeed = 70;
            
            if(blackholeCam.x < 1680 && blackholeCam.x > 0)
            {
                //player to the left
                if(blackholeCam.x + 150 > playerCam->x)
                {
                    //player above
                    if(blackholeCam.y + 150 > playerCam->y)
                    {
                        x1 = abs(bx - px);
                        y1 = abs(by - py);
                        
                        r = sqrt(pow(x1,2) + pow(y1,2)); 	//getting z
                        
                        angleH = acos(x1/r) * 180.0 / PI; 	//getting angle H
						
						z1 = GM*m/(pow(r,2));
						
						Fx = cos(angleH) * z1;
						Fy = sin(angleH) * z1;
						
						angleN = 90 - angleH; 				//getting angle N
                        
						z2 = sqrt(GM/r);
						
                        Vx = cos(angleN) * z2;			//getting y2
                        
                        Vy = sin(angleN) * z2;
                        
                        newX = (abs(Fx) + abs(Vx)) / 2;
                        newY = (abs(Fy) + (-Vy)) / 2;
						
						if(newX > maxSpeed)
						{
							newX = maxSpeed;
						}
						if(newY > maxSpeed)
						{
							newY = maxSpeed;
						}
                        					
						std::cout << "r = " << r << std::endl;
						std::cout << "x1 = " << x1 << std::endl;
						std::cout << "y1 = " << y1 << std::endl;
						std::cout << "angleH = " << angleH << std::endl;
						std::cout << "angleN = " << angleN << std::endl;
						std::cout << "Fx = " << Fx << std::endl;
						std::cout << "Fy = " << Fy << std::endl;
						std::cout << "Vx = " << Vx << std::endl;
						std::cout << "Vy = " << Vy << std::endl;
						std::cout << "newX = " << newX << std::endl;
						std::cout << "newY = " << newY << std::endl;
						std::cout << "----------" << std::endl;

                        
                        //newX = newX + pow(gravAccel, 2);
                        //newY = newY / (newY - 1);
                    }
                    //player below
                    if(blackholeCam.y + 150 < playerCam->y)
                    {
                         x1 = abs(bx - px);
                        y1 = abs(by - py);
                        
                        r = sqrt(pow(x1,2) + pow(y1,2)); 	//getting z
                        
                        angleH = acos(x1/r) * 180.0 / PI; 	//getting angle H
						
						z1 = GM*m/(pow(r,2));
						
						Fx = cos(angleH) * z1;
						Fy = sin(angleH) * z1;
						
						angleN = 90 - angleH; 				//getting angle N
                        
						z2 = sqrt(GM/r);
						
                        Vx = cos(angleN) * z2;			//getting y2
                        
                        Vy = sin(angleN) * z2;
                        
                        newX = (abs(Fx) + (-Vx)) / 2;
                        newY = ((-Fy) + (-Vy)) / 2;
						
						if(newX > maxSpeed)
						{
							newX = maxSpeed;
						}
						if(newY > maxSpeed)
						{
							newY = maxSpeed;
						}
                        
                        //gravAccel++;
                        
                        //newX = newX + pow(gravAccel, 2);
                        //newY = newY / (newY - 1);
                    }
                    
                }
                //player to the right
                else if(blackholeCam.x + 150 < playerCam->x)
                {
                    //player above
                    if(blackholeCam.y + 150 > playerCam->y)
                    {
                         x1 = abs(bx - px);
                        y1 = abs(by - py);
                        
                        r = sqrt(pow(x1,2) + pow(y1,2)); 	//getting z
                        
                        angleH = acos(x1/r) * 180.0 / PI; 	//getting angle H
						
						z1 = GM*m/(pow(r,2));
						
						Fx = cos(angleH) * z1;
						Fy = sin(angleH) * z1;
						
						angleN = 90 - angleH; 				//getting angle N
                        
						z2 = sqrt(GM/r);
						
                        Vx = cos(angleN) * z2;			//getting y2
                        
                        Vy = sin(angleN) * z2;
                        
                        newX = ((-Fx) + abs(Vx)) / 2;
                        newY = (abs(Fy) + abs(Vy)) / 2;
						
						if(newX > maxSpeed)
						{
							newX = maxSpeed;
						}
						if(newY > maxSpeed)
						{
							newY = maxSpeed;
						}
                        
                        //gravAccel++;
                        
                        //newX = newX + pow(gravAccel, 2);
                        //newY = newY / (newY - 1);
                    }
                    //player below
                    if(blackholeCam.y + 150 < playerCam->y)
                    {
                        x1 = abs(bx - px);
                        y1 = abs(by - py);
                        
                        r = sqrt(pow(x1,2) + pow(y1,2)); 	//getting z
                        
                        angleH = acos(x1/r) * 180.0 / PI; 	//getting angle H
						
						z1 = GM*m/(pow(r,2));
						
						Fx = cos(angleH) * z1;
						Fy = sin(angleH) * z1;
						
						angleN = 90 - angleH; 				//getting angle N
                        
						z2 = sqrt(GM/r);
						
                        Vx = cos(angleN) * z2;			//getting y2
                        
                        Vy = sin(angleN) * z2;
                        
                        newX = ((-Fy) + (-Fx)) / 2;
                        newY = ((-Vy) + abs(Vx)) / 2;
						
						if(newX > maxSpeed)
						{
							newX = maxSpeed;
						}
						if(newY > maxSpeed)
						{
							newY = maxSpeed;
						}
                        
                        //gravAccel++;
                        
                        //newX = newX + pow(gravAccel, 2);
                        //newY = newY / (newY - 1);
                    }
                    
                }
                
                xDeltav = xDeltav + newX;
                yDeltav = yDeltav + newY;
                
                //std::cout << "newX = " << newX << std::endl;
                //std::cout << "newY = " << newY << std::endl;


                ply->move(xDeltav, yDeltav, timestep);
                
            }
            
            
        }
