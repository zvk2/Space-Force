#include "HyperStar.h"
#include <iostream>
//const int SCREEN_WIDTH = 1280;
//const int SCREEN_HEIGHT = 720;
	//size of image
	int size = 100;

	HyperStar::HyperStar(SDL_Texture* im, Player* main):starIm(im), ply(main)
	{
		imBox = {0,0,100,100};
		gRenderer = ply->getRend();
		plyCam = ply ->getPlayerCamLoc();
		
		hasShield = ply->shieldStatus();
		getShield = 0;
		head = (struct StarNode*)malloc(sizeof(struct StarNode));
		head->pre = nullptr;
		head->next = nullptr;
		end = head;
	}

	//adds a new star to the list with a random spawn location
	//random angle and random speed
	void HyperStar::addStar()
	{
		//random y
		int upOrDown = rand() % 2; // 0 to 1
		end->next = (struct StarNode*)malloc(sizeof(struct StarNode));

		//random angle
		end->next->angle = (((double)(rand()%50 + 15))/180)*3.14; //10 to 60

		//random speed
		end->next->vel = (rand()%5 + 10)*100.0;

		//one hit only on player
		end->hitPly = false;
		int y;

		//random x
		int x = rand()%426 + 852;
		if(x > 1280)
		{
			x = 1280;
		}
		end->next->math = upOrDown;
		end->next->frame = 0;

		end->next->pre = end;
		end = end->next;
		if(upOrDown == 0)
		{
			y = -size;
		}
		else
		{
			y = 720;
		}

		//square box for star
		end->colTest = {x,y,size,size};
		end->x = (double)x;
		end->y = (double)y;
		end->next = nullptr;
	}

	//places star on screen
	void HyperStar::Render(double timestep)
	{
		if(*hasShield && getShield == 0)
		{
			shield = ply->shieldInteractions();
			getShield = 1;
		}
		if(head->next == nullptr)
		{
			return;
		}
		curr = head->next;
		bool moveOn = true;
		while(curr != nullptr)
		{
			imBox.y = (curr->frame%2)*100;
			curr->frame++;
			SDL_RenderCopy(gRenderer, starIm, &imBox, &curr->colTest);
			if(*hasShield && SDL_HasIntersection(&curr->colTest,shield))
			{
				if(checkShieldCol(curr->colTest))
				{
					killStar();//////////////////
				}
			}
			//checks rectangle intersection first
			if(SDL_HasIntersection(&curr->colTest,plyCam)&& !(curr->hitPly))
			{
				//then checks circle
				if(checkCol(curr->colTest))
				{
					ply->damage(1);
					curr->hitPly = true;//hits player only once
				}
			}
			//if star is off screen free starnode
			if(curr->colTest.x <= -size)
			{
				killStar();
			}
			else if(moveOn)
			{
				if(curr->y>= 720-size && curr->math==0)
				{
					curr->math =1;//star will goes up
				}
				else if(curr->y <=0 && curr->math == 1)
				{
					curr->math =0;//star will go down
				}

				//moves star along angle
				curr->x = curr->x-((curr->vel*timestep)*cos(curr->angle));
				double newY = ((curr->vel*timestep)*sin(curr->angle));
				if(curr->math == 0)
				{
					curr->y = curr->y + newY;
				}
				else
				{
					curr->y = curr->y - newY;
				}
				curr->colTest.x = (int)curr->x;
				curr->colTest.y = (int)curr->y;
				curr = curr->next;
			}
		}

	}
	void HyperStar::killStar()
	{	
		
		if(curr == end)
		{
			end = end->pre;
			end->next = nullptr;
			free(curr);
			curr = nullptr;;
		}
		else
		{
			curr->next->pre = curr->pre;
			curr->pre->next = curr->next;
			StarNode* temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
	//circle collision test
	bool HyperStar::checkCol(SDL_Rect circle)
	{
		int r = size/2;
		int c_x, c_y, d_x, d_y;
		if(circle.x < plyCam-> x)
		{
			c_x = plyCam->x;
		}
		else if(circle.x > plyCam->x + plyCam->w)
		{
			c_x = plyCam->x + plyCam->w;
		}
		else
		{
			c_x = circle.x;
		}

		if(circle.y < plyCam-> y)
		{
			c_y = plyCam->y;
		}
		else if(circle.y > plyCam->y + plyCam->h)
		{
			c_y = plyCam->y + plyCam->h;
		}
		else
		{
			c_y = circle.y;
		}

		d_x = circle.x - c_x;
		d_y = circle.y - c_y;

		int inter =  d_x * d_x + d_y * d_y;
		if(inter <(r * r))
			return true;
		else
			return false;
	}
	
	bool HyperStar::checkShieldCol(SDL_Rect circle)
	{
		return false;
	}
