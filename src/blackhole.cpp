#include "blackhole.h"
#include<stdlib.h>
#include "INC_SDL.h"
#include "Player.h"


blackhole::blackhole(SDL_Texture* textBlackhole, Player* main): ply(main), gBlackhole(textBlackhole)
        {
            gRenderer = ply->getRend();
            blackholeRect = {0, 0, 300, 300};
            blackholeCam = {1680, 720/2, 300, 300};
            blackholeExists = false;
            bFrames = 0;
        }

        bool blackhole::seen()
        {
            return blackholeExists;
        }

        void blackhole::showBlackhole()
        {
            if(blackholeCam.x > -300)
            {
                blackholeCam.x = blackholeCam.x -1;
            }
            else
            {
                blackholeCam = {1580, rand() % (720-300), 300, 300};
            }
            if(blackholeCam.x <= 1580)
            {
                blackholeExists = true;
                SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
            }
            else
            {
                blackholeExists = false;
                return;
            }
            
            if(blackholeExists == true)
            {
                bFrames++;
                
                if (bFrames / 12 > 5)
                {
                    bFrames = 0;
                }

                blackholeRect.x = (bFrames / 12) * 300;
            }
            
        }

