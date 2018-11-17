#include "HyperStar.h"
#include <iostream>
//const int SCREEN_WIDTH = 1280;
//const int SCREEN_HEIGHT = 720;
	int size = 100;
	HyperStar::HyperStar(SDL_Texture* im, Player* main):starIm(im), ply(main)
	{
		imBox = {0,0,100,100};
		gRenderer = ply->getRend();
		plyCam = ply ->getPlayerCamLoc();
		head = (struct StarNode*)malloc(sizeof(struct StarNode));
		head->pre = nullptr;
		head->next = nullptr;
		end = head;
	}

	
	void HyperStar::addStar()
	{
		
		int upOrDown = rand() % 2; // 0 to 1
		end->next = (struct StarNode*)malloc(sizeof(struct StarNode));
		end->next->angle = (((double)(rand()%50 + 15))/180)*3.14; //10 to 60
		end->next->vel = (rand()%5 + 10)*100.0;
		int y;
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
		end->colTest = {x,y,size,size};
		end->x = (double)x;
		end->y = (double)y;
		end->next = nullptr;
	}
	
	void HyperStar::Render(double timestep)
	{
		if(head->next == nullptr)
		{
			return;
		}
		StarNode* curr = head->next;
		StarNode* temp;
		while(curr != nullptr)
		{
			imBox.y == (curr->frame%2)*100;
			curr->frame++;
			SDL_RenderCopy(gRenderer, starIm, &imBox, &curr->colTest);
			if(SDL_HasIntersection(&curr->colTest,plyCam))
			{
				checkCol(curr->colTest);
			}
			if(curr->colTest.x <= -size)
			{
				if(curr == end)
				{
					end = end->pre;
					end->next = nullptr;
					free(curr);
					curr = nullptr;
				}
				else
				{
					
					curr->next->pre = curr->pre;
					curr->pre->next = curr->next;
					temp = curr;
					curr = curr->next;
					free(temp);
				}
			}
			else
			{
				if(curr->y>= 720-size && curr->math==0)
				{
					curr->math =1;
				}
				else if(curr->y <=0 && curr->math == 1)
				{
					curr->math =0;
				}
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
	void HyperStar::checkCol(SDL_Rect inter)
	{
		int r = size/2;
		int c_x, c_y, d_x, d_y;
	}