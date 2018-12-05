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

			attackVector = std::vector<RenderObject*>();
		}
		void attack::setColSound(music* sounds)
		{
			mus = sounds;
		}
		void attack::setAttack(SDL_Rect* attac)
		{
			attackBox = attac;
			//~ gAttack = gAtt;
			cam.w = attac->w;
			cam.h = attac->h;
		}

        //Get the head of the list
        struct Node* attack::getHead()
        {
            return head;
        }


		// new attacks are added to the end of the list
		void attack::addAttack(int x, int y, int image)
		{
			cam.x = x;
			cam.y = y;
			if(image == 1)
			{
				attackVector.push_back(
					new RenderObject(
						x, y, 1, openGL->allBufferAttributes["resources/imgs/attack.png"]
					)
				);
			}
			else
			{
				attackVector.push_back(
					new RenderObject(
						x, y, 1, openGL->allBufferAttributes["resources/imgs/missile.png"]
					)
				);
			}

			openGL->AppendRenderObject(attackVector[attackVector.size() - 1]);
		}

		//when the currently first attack hits the end of the screen than
		//it will free that information and delete it from the list
		//then it will continue to render all other attacks further across the screen
		void attack::renderAttack(double timestep, int goLeft)
		{
			int xDisplacement = (int) (1000 * timestep);

			int index = 0;
			int vectorSize = attackVector.size();
			while(index < vectorSize)
			{
				RenderObject* currentAttack = attackVector[index];
				//~ std::cout << currentAttack->index << std::endl;
				if(goLeft){
					currentAttack->ChangeCoordinates(
						currentAttack->x - xDisplacement,
						currentAttack->y,
						currentAttack->z
					);
				}
				else{
					currentAttack->ChangeCoordinates(
						currentAttack->x + xDisplacement,
						currentAttack->y,
						currentAttack->z
					);
				}

				// Doing this check every time might be overkill
				if (currentAttack->x >= SCREEN_WIDTH)
				{
					openGL->RemoveRenderObject(currentAttack->index);
					//~ std::cout << "Attack Die" << std::endl;

					attackVector.erase(attackVector.begin() + index);
					vectorSize = attackVector.size();
				}
				else {
					index += 1;
				}

				vectorSize = attackVector.size();
			}
		}

		// TODO TODO TODO
		int attack::hitIntersect(SDL_Rect* rect)
		{
			int count = 0;
			int index = 0;
			int vectorSize = attackVector.size();
			while(index < vectorSize)
			{
				RenderObject* currentAttack = attackVector[index];

				SDL_Rect currentRect = {
					(int)currentAttack->x,
					(int)currentAttack->y,
					(int)currentAttack->bufferAttributes.width,
					(int)currentAttack->bufferAttributes.height
				};

				if(SDL_HasIntersection(rect, &currentRect))
				{
					//mus->yourAttackHits();
					openGL->RemoveRenderObject(currentAttack->index);
					//~ std::cout << "Attack Die" << std::endl;

					attackVector.erase(attackVector.begin() + index);
					vectorSize = attackVector.size();
					//////////////
					count++;
				}
				else
				{
					index += 1;
				}

				vectorSize = attackVector.size();
			}

			return count;
		}
