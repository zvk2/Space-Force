#include "attack.h"

		attack::attack(SDL_Renderer* Renderer, SDL_Texture* gAtt, SDL_Rect* attac,SDL_Rect cam):attackBox{attac}, gRenderer{Renderer}, gAttack{gAtt}
		{
			head = (struct Node*)malloc(sizeof(struct Node));
			head->attackCam = cam;
			end = head;
			end->next = nullptr;
		}
		void attack::addAttack(SDL_Rect cam)
		{
			end->next = (struct Node*)malloc(sizeof(struct Node));
			end ->next->attackCam = cam;
			end = end->next;
			SDL_RenderCopy(gRenderer, gAttack, attackBox, &end->attackCam);
			end->next = nullptr;
		}
		void attack::renderAttack(double timestep)
		{
			curr = head->next;
			if(curr != nullptr && head -> attackCam.x + 80 >= 1280)
			{
				curr = head->next;
				free(head);
				head = curr;
			}
		
			while(curr != nullptr)
			{
				curr->attackCam.x +=(int) (3000 * timestep);
				SDL_RenderCopy(gRenderer, gAttack, attackBox, &curr->attackCam);
				curr = curr->next;
			}
		}