#include "attack.h"
		//will keep track of all attacks on screen

		attack::attack(SDL_Renderer* Renderer):gRenderer{Renderer}
		{
			head = (struct Node*)malloc(sizeof(struct Node));
			cam = {0,0,0,0};
			end = head;
			end->next = nullptr;
		}
		void attack::setAttack(SDL_Texture* gAtt, SDL_Rect* attac)
		{
			attackBox = attac;
			gAttack = gAtt;
			cam.w = attac->w;
			cam.h = attac->h;
		}
		//new attacks are added to the end of the list
		void attack::addAttack(int x, int y)
		{
			cam.x = x;
			cam.y = y;
			end->next = (struct Node*)malloc(sizeof(struct Node));
			end ->next->attackCam = cam;
			end = end->next;
			SDL_RenderCopy(gRenderer, gAttack, attackBox, &end->attackCam);
			end->next = nullptr;
		}
		//when the currently first attack hits the end of the screen than
		//it will free that information and delete it from the list
		//then it will continue to render all other attacks further across the screen
		void attack::renderAttack(double timestep)
		{
			Node* pre = head;
			curr = head->next;
			while(curr != nullptr)
			{
				curr->attackCam.x +=(int) (1000 * timestep);
				SDL_RenderCopy(gRenderer, gAttack, attackBox, &curr->attackCam);
				pre = curr;
				curr = curr->next;
			}
			curr = head->next;
			if(curr != nullptr && curr -> attackCam.x>= 1280)
			{

				head->next = curr->next;
				if(curr == end)
				{
					end = head;
				}
				free(curr);
			}
		}
		//will count how many times an attack hit that object
		//and delete that attack
		int attack::hitIntersect(SDL_Rect* rect)
		{
			Node* pre = head;
			//~ Node* temp;
			curr = head->next;
			int count = 0;
			while(curr != nullptr)
			{
				if(SDL_HasIntersection(rect, &curr->attackCam))
				{
					//~ temp = curr;
					if(curr == end)
					{
						end = pre;
						curr = nullptr;
					}
					else
					{
						curr = curr->next;
						pre->next = curr;
					}
					//~ free(temp);
					count++;
				}
				else
				{
					pre = curr;
					curr = curr->next;
				}
			}
			return count;
		}
