#include "Duo.hpp"

#ifdef __APPLE__
mat4 x_rot_matrix(GLfloat theta)
{
    mat4 rmat = identity_matrix();
    
    rmat.y.y = cos(theta);
    rmat.y.z = sin(theta);
    rmat.z.y = -sin(theta);
    rmat.z.z = cos(theta);
    
    return rmat;
}

mat4 y_rot_matrix(GLfloat theta)
{
    mat4 rmat = identity_matrix();
    
    rmat.x.x = cos(theta);
    rmat.x.z = -sin(theta);
    rmat.z.x = sin(theta);
    rmat.z.z = cos(theta);
    
    return rmat;
}

mat4 z_rot_matrix(GLfloat theta)
{
    mat4 rmat = identity_matrix();
    
    rmat.x.x = cos(theta);
    rmat.x.y = sin(theta);
    rmat.y.x = -sin(theta);
    rmat.y.y = cos(theta);
    
    return rmat;
}


mat4 m_mult(mat4 a, mat4 b) {
    mat4 rmat;
    
    rmat.x.x = (a.x.x * b.x.x) + (a.x.y * b.y.x) + (a.x.z * b.z.x) + (a.x.w * b.w.x);
    rmat.y.x = (a.y.x * b.x.x) + (a.y.y * b.y.x) + (a.y.z * b.z.x) + (a.y.w * b.w.x);
    rmat.z.x = (a.z.x * b.x.x) + (a.z.y * b.y.x) + (a.z.z * b.z.x) + (a.z.w * b.w.x);
    rmat.w.x = (a.w.x * b.x.x) + (a.w.y * b.y.x) + (a.w.z * b.z.x) + (a.w.w * b.w.x);
    
    rmat.x.y = (a.x.x * b.x.y) + (a.x.y * b.y.y) + (a.x.z * b.z.y) + (a.x.w * b.w.y);
    rmat.y.y = (a.y.x * b.x.y) + (a.y.y * b.y.y) + (a.y.z * b.z.y) + (a.y.w * b.w.y);
    rmat.z.y = (a.z.x * b.x.y) + (a.z.y * b.y.y) + (a.z.z * b.z.y) + (a.z.w * b.w.y);
    rmat.w.y = (a.w.x * b.x.y) + (a.w.y * b.y.y) + (a.w.z * b.z.y) + (a.w.w * b.w.y);
    
    rmat.x.z = (a.x.x * b.x.z) + (a.x.y * b.y.z) + (a.x.z * b.z.z) + (a.x.w * b.w.z);
    rmat.y.z = (a.y.x * b.x.z) + (a.y.y * b.y.z) + (a.y.z * b.z.z) + (a.y.w * b.w.z);
    rmat.z.z = (a.z.x * b.x.z) + (a.z.y * b.y.z) + (a.z.z * b.z.z) + (a.z.w * b.w.z);
    rmat.w.z = (a.w.x * b.x.z) + (a.w.y * b.y.z) + (a.w.z * b.z.z) + (a.w.w * b.w.z);
    
    rmat.x.w = (a.x.x * b.x.w) + (a.x.y * b.y.w) + (a.x.z * b.z.w) + (a.x.w * b.w.w);
    rmat.y.w = (a.y.x * b.x.w) + (a.y.y * b.y.w) + (a.y.z * b.z.w) + (a.y.w * b.w.w);
    rmat.z.w = (a.z.x * b.x.w) + (a.z.y * b.y.w) + (a.z.z * b.z.w) + (a.z.w * b.w.w);
    rmat.w.w = (a.w.x * b.x.w) + (a.w.y * b.y.w) + (a.w.z * b.z.w) + (a.w.w * b.w.w);
    
    return rmat;
}
#endif

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
