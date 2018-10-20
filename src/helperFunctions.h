// Author: Luke Malchow Bergenthal
// Helper functions
#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
// #include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

// Often used
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define PI 3.14159265359

#include <math.h>
#include "shader.h"

// Definition of a vec3 (vector of size 3)
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3;

// Definition of a vec4 (vector of size 4)
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

// Definition of a mat4 (matrix of size 4x4)
//~ typedef struct
//~ {
    //~ GLfloat x->x; GLfloat x->y;GLfloat x->z; GLfloat x->w;
    //~ GLfloat y->x; GLfloat y->y;GLfloat y->z; GLfloat y->w;
    //~ GLfloat z->x; GLfloat z->y;GLfloat z->z; GLfloat z->w;
    //~ GLfloat w->x; GLfloat w->y;GLfloat w->z; GLfloat w->w;
//~ } mat4;

typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

// Defines a material
typedef struct
{
vec4 reflect_ambient;
vec4 reflect_diffuse;
vec4 reflect_specular;
GLfloat shininess;
} material;

void v4_print(vec4 *v);
void v4v4_addition(vec4 *v1, vec4 *v2, vec4 *result);
void v4v4_subtraction(vec4 *v1, vec4 *v2, vec4 *result);
void v4v4_multiplication(vec4 *v1, vec4 *v2, vec4 *result);
void v4_scalar_multiplication(GLfloat s, vec4 *v, vec4 *result);
void v4_cross_product(vec4 *v1, vec4 *v2, vec4 *result);
GLfloat v4_dot_product(vec4 *v1, vec4 *v2);
void mat4_print(mat4 *m);
void mat4_scalar_multiplication(GLfloat s, mat4 *m, mat4 *result);
void mat4_addition(mat4 *m1, mat4 *m2, mat4 *result);
void mat4_subtraction(mat4 *m1, mat4 *m2, mat4 *result);
void mat4_multiplication(mat4 *m1, mat4 *m2, mat4 *result);

void mat4_v4_multiplication(vec4 *v, mat4 *m, vec4 *result);
void v4_mat4_multiplication(vec4 *v, mat4 *m, vec4 *result);

void mat4_transpose(mat4 *m, mat4 *result);
void mat4_minor(mat4 *m, mat4 *result);
void mat4_cofactor(mat4 *m, mat4 *result);
void mat4_inverse(mat4 *m, mat4 *result);

GLfloat v4_magnitude(vec4 *v);
GLfloat v4v4_rads_between(vec4 *v1, vec4 *v2);

void unit_vector(vec4 *v, vec4 *result);
void unit_vector_from_two_points(vec4 *p1, vec4 *p2, vec4 *result);

void get_rotation_matrix_to_x(vec4 *v, mat4 *result);
void get_rotation_matrix_to_y(vec4 *v, mat4 *result);
void get_rotation_matrix_back_from_x(vec4 *v, mat4 *result);
void get_rotation_matrix_back_from_y(vec4 *v, mat4 *result);

void get_rotation_matrix_about_x(GLfloat theta, mat4 *result);
void get_rotation_matrix_about_y(GLfloat theta, mat4 *result);
void get_rotation_matrix_about_z(GLfloat theta, mat4 *result);

//~ void get_rotation_matrix_about_y_with_offset(
    //~ GLfloat theta_rads,
    //~ GLfloat x_offset, 
    //~ GLfloat y_offset, 
    //~ GLfloat z_offset, 
    //~ mat4 *result
//~ );

void get_rotation_matrix_about_origin(GLfloat theta, vec4 *v, mat4 *result);

mat4 identity_matrix();
mat4 empty_matrix();

mat4 look_at(
    GLfloat eyex, GLfloat eyey, GLfloat eyez,
    GLfloat atx, GLfloat aty, GLfloat atz,
    GLfloat upx, GLfloat upy, GLfloat upz
);

mat4 ortho(
    GLfloat left, GLfloat right,
    GLfloat bottom, GLfloat top,
    GLfloat near, GLfloat far
);

mat4 frustum(
    GLfloat left, GLfloat right,
    GLfloat bottom, GLfloat top,
    GLfloat near, GLfloat far
);
