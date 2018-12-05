#ifndef SHAPES_H
#define SHAPES_H

#include <math.h>
#include <stdlib.h>
#include "matrix.h"

vec4 v4(GLfloat, GLfloat, GLfloat, GLfloat);
vec4* sphere(GLfloat, int, int, int*);
vec4* cube(int*);
vec4* circle(int*);
vec4* cone(int*);

#endif
