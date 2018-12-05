//
//  VirtualPeacefulKing.cpp
//  CS1666_SpaceForce
//
//

#include "VirtualPeacefulKing.h"

//Public methods


VirtualPeacefulKing::VirtualPeacefulKing(OpenGLRenderer* gRend, int initialHealth, int attack,int skillCD, Player* main):hitPoints(initialHealth),attackPower(attack),phys(0,0,300.0,3600.0),missile(gRend),xCoord(1280/10),yCoord(720/2),cd(skillCD), ply(main)
{
	openGL = gRend;
    kingRect = {0, 0, 288, 288};
    kingCam = {1280/2, 720/2, 288, 288};
	plyCam = ply-> getPlayerCamLoc();
	SDL_Rect attackBox = {0,0,70,48};
	missile.setAttack(&attackBox);

	//~ std::string initTexture = "resources/imgs/King.png";
	//~ kingTexture = initTexture.c_str();

	render = new RenderObject(
		kingCam.x, kingCam.y, 0.5, openGL->allBufferAttributes["resources/imgs/King.png"]
	);

	openGL->AppendRenderObject(render);
}

//Causing damage to the boss
void VirtualPeacefulKing::LostHealth(int damage)
{
    minusHealth(damage);
}

//set the current position of the king
void VirtualPeacefulKing::setPosition(double x, double y, bool showTime)
{
    xCoord = x;
    yCoord = y;

    if (showTime)
    {
        checkBoundary();
    }
    

    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;

    render->ChangeCoordinates(
		kingCam.x,
		kingCam.y,
		render->z
    );
}

//set the current velocity of the king
void VirtualPeacefulKing::setVelocity(double x, double y)
{
    phys.setxVelocity(x);
    phys.setyVelocity(y);
}

//Check the boundary so that our king will not move outside the screen
void VirtualPeacefulKing::checkBoundary()
{
    if (kingCam.x < 0)
        kingCam.x = 0;
    if (kingCam.x + kingCam.w > SCREEN_WIDTH)
        kingCam.x = SCREEN_WIDTH - kingCam.w;
    if (kingCam.y <= 0)
	{
		kingCam.y = 0;
		setVelocity(0, 10);
		delta = 1;
	}
    if (kingCam.y + kingCam.h >= SCREEN_HEIGHT)
	{
        kingCam.y = SCREEN_HEIGHT - kingCam.h;
		setVelocity(0,-10);
		delta = -1;
	}
	if(SDL_HasIntersection(&kingCam, plyCam))
	{
		int x;
		int y;
		y = plyCam->y;
		if(plyCam->x+plyCam->w > kingCam.x)
		{
			x =  plyCam->w + plyCam->x - kingCam.x;
			x = plyCam->x-x;
		}
		ply->setPosition(x,y);
	}

    render->ChangeCoordinates(
		kingCam.x,
		kingCam.y,
		render->z
    );
}

//Move the king
void VirtualPeacefulKing::move(double step,bool showTime)

{
    phys.ChangeVelocity(0, delta, step);

    xCoord += (phys.getxVelocity() * step);
    yCoord += (phys.getyVelocity() * step);

    //Our king will not move pass the screen
    if (showTime)
    {
        checkBoundary();
    }
    
    
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;

    render->ChangeCoordinates(
		kingCam.x,
		kingCam.y,
		render->z
    );
	if(SDL_GetTicks()%1000 < 50)
	{
		missile.addAttack(kingCam.x,kingCam.y+(kingCam.h)/3,2);
	}
	missile.renderAttack(-step,0);
}

//Animate the king
void VirtualPeacefulKing::animate(int frames)
{
    //~ kingRect.x = ((frames / 10) % 6) * kingRect.w;
    render->IterateFrame();
}

//Some accessors
double VirtualPeacefulKing::getXPosition()
{
    return xCoord;
}

double VirtualPeacefulKing::getYPosition()
{
    return yCoord;
}

SDL_Rect VirtualPeacefulKing::getRect()
{
    return kingRect;
}



//~ SDL_Texture* VirtualPeacefulKing::getSheet()
//~ {
    //~ return kingSheet;
//~ }

SDL_Rect VirtualPeacefulKing::getCamera()
{
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;
    return kingCam;
}

SDL_Rect* VirtualPeacefulKing::getCameraLoc()
{
    kingCam.x = (int) xCoord;
    kingCam.y = (int) yCoord;
    return &kingCam;
}

bool VirtualPeacefulKing::checkRectCollision(SDL_Rect *a, SDL_Rect *b)
{
    // Check vertical overlap
    if (a->y + a->h <= b->y)
        return false;
    if (a->y >= b->y + b->h)
        return false;
    
    // Check horizontal overlap
    if (a->x >= b->x + b->w)
        return false;
    if (a->x + a->w <= b->x)
        return false;
    
    // Must overlap in both
    return true;
}



//private methods
void VirtualPeacefulKing::minusHealth(int amount)
{
    hitPoints -= amount;
}

void VirtualPeacefulKing::plusHealth(int amount)
{
    hitPoints += amount;
}









