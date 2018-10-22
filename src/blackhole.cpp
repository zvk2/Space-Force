#include "blackhole.h"
#include<stdlib.h>
#include "INC_SDL.h"

        blackhole::blackhole(SDL_Renderer* Renderer, SDL_Texture* gBlackhole, SDL_Rect* blackholeRect,SDL_Rect blackholeCam)
        {
            
        }

        void blackhole::showBlackhole(SDL_Rect blackholeCam)
        {
            SDL_RenderCopy(gRenderer, gBlackhole, blackholeRect, &blackholeCam);
        }

