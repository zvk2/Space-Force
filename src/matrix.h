#include <stdio.h>
#include "../opengl/headers/GL/glew.h"

typedef struct vec4 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} vec4;

typedef struct mat4 {
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

void print_vec4(vec4 vect); // print a 4x1 vec4

void print_mat4(mat4 mat); // print a 4x4 matrix

vec4 v_add(vec4 a, vec4 b); // vec4-vec4 addition

vec4 v_sub(vec4 a, vec4 b); // vec4-vec4 subtraction

vec4 svect_mult(GLfloat alpha, vec4 v); // scalar vector multiplication

GLfloat v_dot(vec4 a, vec4 b); // vector dot product

vec4 v_cross(vec4 a, vec4 b); // vector cross product

mat4 smat_mult(GLfloat alpha, mat4 m); // scalar matrix multiplication

mat4 m_add(mat4 m, mat4 n); // matrix addition

mat4 m_sub(mat4 m, mat4 n); // matrix subtraction

mat4 m_mult(mat4 a, mat4 b); // matrix multiplication

mat4 transpose(mat4 a); // matrix transposition

int inverse(mat4 a, mat4* rmat); // matrix inversion

vec4 mat_vec_mult(mat4 m, vec4 n); // matrix * vector multiplication