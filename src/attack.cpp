#include "attack.h"
		//will keep track of all attacks on screen

		attack::attack(OpenGLRenderer* Renderer):openGL{Renderer}
		{
			//~ head = (struct Node*)malloc(sizeof(struct Node));
			// Head is a contrived node?
			head = new struct Node();
			cam = {0,0,0,0};
			end = head;
			end->next = nullptr;
			end->pre = nullptr;
		}
		void attack::setAttack(SDL_Rect* attac)
		{
			attackBox = attac;
			//~ gAttack = gAtt;
			cam.w = attac->w;
			cam.h = attac->h;
		}
		//new attacks are added to the end of the list
		void attack::addAttack(int x, int y)
		{
			cam.x = x;
			cam.y = y;
			//~ end->next = (struct Node*)malloc(sizeof(struct Node));
			//~ end >next->attackCam = cam;
			//~ end = end->next;
			//~ SDL_RenderCopy(gRenderer, gAttack, attackBox, &end->attackCam);
			//~ end->next = nullptr;

			end->next = new struct Node();

			end->next->render = new RenderObject(
				x, y, 1, openGL->allBufferAttributes["resources/imgs/attack.png"]
			);

			openGL->AppendRenderObject(end->next->render);

			end->next->attackCam = cam;

			end->next->pre = end;

			end = end->next;
			end->next = nullptr;
		}
		//when the currently first attack hits the end of the screen than
		//it will free that information and delete it from the list
		//then it will continue to render all other attacks further across the screen
		void attack::renderAttack(double timestep)
		{
			int xDisplacement = (int) (1000 * timestep);
			//~ Node* pre = head;
			Node* temp;
			curr = head->next;
			while(curr != nullptr)
			{
				curr->attackCam.x += xDisplacement;
				curr->render->ChangeCoordinates(
					curr->attackCam.x,
					curr->render->y,
					curr->render->z
				);

				// Doing this check every time might be overkill
				if (curr != nullptr && curr->render->x >= SCREEN_WIDTH)
				{
					curr->pre->next = curr->next;
					if (curr->next)
					{
						curr->next->pre = curr->pre;
					}
					else
					{
						end = curr->pre;
						end->next = nullptr;
					}

					temp = curr;

					openGL->RemoveRenderObject(temp->render->index);
					std::cout << "Attack Die" << std::endl;

					delete temp;
				}
				//~ else
				//~ {
					//~ pre = curr;
				//~ }
				curr = curr->next;
			}
			//~ curr = head->next;
			//~ if(curr != nullptr && curr->render->x >= SCREEN_WIDTH)
			//~ {
				//~ head->next = curr->next;
				//~ if(curr == end)
				//~ {
					//~ end = head;
				//~ }
				//~ free(curr);
			//~ }
		}
		//will count how many times an attack hit that object
		//and delete that attack
		int attack::hitIntersect(SDL_Rect* rect)
		{
			//~ Node* pre = head;
			Node* temp;
			curr = head->next;
			int count = 0;
			while(curr != nullptr)
			{
				if(SDL_HasIntersection(rect, &curr->attackCam))
				{
					temp = curr;
					if(!curr->next)
					{
						end = curr->pre;
						end->next = nullptr;
					}
					else
					{
						curr->pre->next = curr->next;
						curr = curr->next;
					}

					openGL->RemoveRenderObject(temp->render->index);
					std::cout << "Attack Die" << std::endl;
					delete temp;

					count++;
				}
				else
				{
					curr = curr->next;
				}
			}

			return count;
		}
