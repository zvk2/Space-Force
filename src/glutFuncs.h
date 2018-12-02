#ifndef GLUTFUNCS_H
#define GLUTFUNCS_H

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "matrix.h"

void display();
void idle();
void keyboard(unsigned char, int, int);
void reshape(int, int);

extern mat4 view, ctm;
extern GLuint look_location, ctm_location;
extern GLfloat eyex, eyey, eyez, atx, aty, atz;
extern int num_vertices;

#endif
