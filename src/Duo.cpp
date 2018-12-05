#include "Duo.hpp"

Duo::Duo(OpenGLRenderer *gRenderer, Player* p):
	ply(p)
{
	openGL = ply->getRend();

	miasmaRect = {300, 2000, 250, 250};

	// There will only be one render object
	renderMiasma = new RenderObject(
		miasmaRect.x, miasmaRect.y, -0.9, openGL->allBufferAttributes["resources/imgs/miasma.png"]
	);
	openGL->AppendRenderObject(renderMiasma);

	// There will only be one render object
	renderChatterBox = new RenderObject(
		miasmaRect.x, miasmaRect.y, 0, openGL->allBufferAttributes["chatter_box"]
	);
	openGL->AppendRenderObject(renderChatterBox);

	// There will only be one render object
	renderHarbinger = new RenderObject(
		miasmaRect.x, miasmaRect.y, 0, openGL->allBufferAttributes["harbinger"]
	);
	openGL->AppendRenderObject(renderHarbinger);

	currentRot = 0;
	GetNewLocation();
}

void Duo::Move()
{
	if (miasmaRect.y < -miasmaRect.h)
	{
		GetNewLocation();
	}
	miasmaRect.y -= 2;

	renderHarbinger->ChangeCoordinates(
		SCREEN_WIDTH / 2 - 125,
		SCREEN_HEIGHT / 2 - 125,
		0
	);

	mat4 rot = m_mult(renderHarbinger->ctm, y_rot_matrix(currentRot));

	renderHarbinger->ChangeCoordinates(
		miasmaRect.x - 500,
		miasmaRect.y - SCREEN_HEIGHT * 1.8,
		0
	);

	renderHarbinger->ctm = m_mult(
		rot,
		renderHarbinger->ctm
	);

	renderChatterBox->ChangeCoordinates(
		SCREEN_WIDTH / 2 - 125,
		SCREEN_HEIGHT / 2 - 125,
		0
	);

	rot = m_mult(y_rot_matrix(currentRot), z_rot_matrix(currentRot));
	rot.x.x *= 0.25;
	rot.y.y *= 0.25;
	rot.z.z *= 0.25;
	rot = m_mult(x_rot_matrix(currentRot), rot);
	rot = m_mult(renderChatterBox->ctm, rot);

	renderChatterBox->ChangeCoordinates(
		miasmaRect.x - 500,
		miasmaRect.y - 250,
		0
	);

	renderChatterBox->ctm = m_mult(
		rot,
		renderChatterBox->ctm
	);

	// Miasma
	renderMiasma->ChangeCoordinates(
		miasmaRect.x,
		miasmaRect.y,
		renderMiasma->z
	);

	renderMiasma->IterateFrame();

	// Check player collision
	if (SDL_HasIntersection(&miasmaRect, ply->getPlayerCamLoc()))
	{
		ply->LostHealth(1);
		ply->damage(1);
	}

	currentRot += 0.05;

	if (currentRot > 3.14159265 * 2) {
		currentRot = 0;
	}
}

// Get new location
void Duo::GetNewLocation()
{
	srand(time(0));
	int randX = rand() % SCREEN_WIDTH;
	int randY = rand() % 4000 + 2000;

	miasmaRect.x = randX;
	miasmaRect.y = randY;
}
