#include "attack.h"
		//will keep track of all attacks on screen

		attack::attack(OpenGLRenderer* Renderer):openGL{Renderer}
		{
			std::string initTexture = "resources/imgs/attack.png";
			attackTexture = initTexture.c_str();

			//~ head = (struct Node*)malloc(sizeof(struct Node));
			// Head is a contrived node?
			head = new struct Node();
			cam = {0,0,0,0};
			end = head;
			end->next = nullptr;
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

			RenderObject *newAttack = new RenderObject(
				x, y, -1, openGL->allBufferAttributes[attackTexture]
			);

			openGL->AppendRenderObject(newAttack);

			end->next = new struct Node();
			end->next->render = newAttack;
			end->next->attackCam = cam;

			end = end->next;
			end->next = nullptr;
		}
		//when the currently first attack hits the end of the screen than
		//it will free that information and delete it from the list
		//then it will continue to render all other attacks further across the screen
		void attack::renderAttack(double timestep)
		{
			int xDisplacement = (int) (1000 * timestep);
			Node* pre = head;
			curr = head->next;
			while(curr != nullptr)
			{
				curr->attackCam.x += xDisplacement;
				curr->render->ChangeCoordinates(
					curr->render->x + xDisplacement,
					curr->render->y,
					curr->render->z
				);

				// Doing this check every time might be overkill
				if (curr != nullptr && curr->render->x >= SCREEN_WIDTH)
				{
					pre->next = curr->next;

					openGL->RemoveRenderObject(curr->render->index);

					delete curr;
				}
				else
				{
					pre = curr;
				}

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
			Node* pre = head;
			Node* temp;
			curr = head->next;
			int count = 0;
			while(curr != nullptr)
			{
				if(SDL_HasIntersection(rect, &curr->attackCam))
				{
					temp = curr;
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

					openGL->RemoveRenderObject(temp->render->index);
					delete temp;

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
