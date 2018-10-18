#include "attack.h"
		//will keep track of all attacks on screen
		attack::attack(SDL_Renderer* Renderer, SDL_Texture* gAtt, SDL_Rect* attac,SDL_Rect cam):attackBox{attac}, gRenderer{Renderer}, gAttack{gAtt}
		{
			head = (struct Node*)malloc(sizeof(struct Node));
			head->attackCam = cam;
			end = head;
			end->next = nullptr;
		}
		//new attacks are added to the end of the list
		void attack::addAttack(SDL_Rect cam, bool move)
		{
			end->next = (struct Node*)malloc(sizeof(struct Node));
			if(!move)
				cam.x -= 305;
			end ->next->attackCam = cam;
			end = end->next;
			end->face = move;
			SDL_RenderCopy(gRenderer, gAttack, attackBox, &end->attackCam);
			
			end->next = nullptr;
		}
		//when the currently first attack hits the end of the screen than 
		//it will free that information and delete it from the list
		//then it will continue to render all other attacks further across the screen
		void attack::renderAttack(double timestep)
		{
			curr = head->next;
			if(curr != nullptr && (head -> attackCam.x >= 1280 || head->attackCam.x <= 0))
			{
				curr = head->next;
				free(head);
				head = curr;
			}
		
			while(curr != nullptr)
			{
				if(curr -> face)
					curr->attackCam.x +=(int) (3000 * timestep);
				else
					curr->attackCam.x -=(int) (3000 * timestep);
				SDL_RenderCopy(gRenderer, gAttack, attackBox, &curr->attackCam);
				curr = curr->next;
			}
		}