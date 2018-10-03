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

#include <stdio.h>
#include "helperFunctions.h"

//~ void v4_print(vec4 *v) {
    //~ printf("[%f, %f, %f, %f]", v->x, v->y, v->z, v->w);
//~ }

void v4_print(vec4 *v) {
    printf("[%f]\n[%f]\n[%f]\n[%f]\n", v->x, v->y, v->z, v->w);
}

void v4v4_addition(vec4 *v1, vec4 *v2, vec4 *result) {
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    // Not that this matters
    result->w = v1->w + v2->w;
    // result->w = v1->w;
}

void v4v4_subtraction(vec4 *v1, vec4 *v2, vec4 *result) {
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    // Not that this matters
    result->w = v1->w - v2->w;
    // result->w = v1->w;
}

// AMBIGUOUS, IS THIS WHAT WAS INTENDED?
void v4v4_multiplication(vec4 *v1, vec4 *v2, vec4 *result) {
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    // Not that this matters
    result->w = v1->w * v2->w;
    // result->w = v1->w;
}

// NOTE THAT THE SCALAR IS A GLfloat
void v4_scalar_multiplication(GLfloat s, vec4 *v, vec4 *result) {
    result->x = v->x * s;
    result->y = v->y * s;
    result->z = v->z * s;
    // Not that this matters
    result->w = v->w * s;
    // result->w = v->w;
}

// This will use the Sarrus rule method (sans an actual matrix)
void v4_cross_product(vec4 *v1, vec4 *v2, vec4 *result) {
    result->x = v1->y * v2->z - v1->z * v2->y;
    result->y = v1->z * v2->x - v1->x * v2->z;
    result->z = v1->x * v2->y - v1->y * v2->x;
    // Arbitrary set to 0 (if v1 properly set)
    // result->w = v1->w;
    result->w = 0;
}

// BE SURE TO TEST THIS
GLfloat v4_dot_product(vec4 *v1, vec4 *v2) {
    GLfloat result = 0.0;
    
    result += v1->x * v2->x;
    result += v1->y * v2->y;
    result += v1->z * v2->z;
    // No point in adding w?
    // result += v1->w * v2->w;
    
    return result;
}

void mat4_print(mat4 *m) {
    //~ printf("[%f, %f, %f, %f;", m->x.x, m->x.y, m->x.z, m->x.w);
    //~ printf(" %f, %f, %f, %f;", m->y.x, m->y.y, m->y.z, m->y.w);
    //~ printf(" %f, %f, %f, %f;", m->z.x, m->z.y, m->z.z, m->z.w);
    //~ printf(" %f, %f, %f, %f]", m->w.x, m->w.y, m->w.z, m->w.w);
    printf("[[%f][%f][%f][%f]]\n", m->x.x, m->y.x, m->z.x, m->w.x);
    printf("[[%f][%f][%f][%f]]\n", m->x.y, m->y.y, m->z.y, m->w.y);
    printf("[[%f][%f][%f][%f]]\n", m->x.z, m->y.z, m->z.z, m->w.z);
    printf("[[%f][%f][%f][%f]]\n", m->x.w, m->y.w, m->z.w, m->w.w);
}

// NOTE THAT THE SCALAR IS A GLfloat
void mat4_scalar_multiplication(GLfloat s, mat4 *m, mat4 *result) {
    result->x.x = m->x.x * s;
    result->x.y = m->x.y * s;
    result->x.z = m->x.z * s;
    result->x.w = m->x.w * s;
    result->y.x = m->y.x * s;
    result->y.y = m->y.y * s;
    result->y.z = m->y.z * s;
    result->y.w = m->y.w * s;
    result->z.x = m->z.x * s;
    result->z.y = m->z.y * s;
    result->z.z = m->z.z * s;
    result->z.w = m->z.w * s;
    result->w.x = m->w.x * s;
    result->w.y = m->w.y * s;
    result->w.z = m->w.z * s;
    result->w.w = m->w.w * s;
}

void mat4_addition(mat4 *m1, mat4 *m2, mat4 *result) {
    result->x.x = m1->x.x + m2->x.x;
    result->x.y = m1->x.y + m2->x.y;
    result->x.z = m1->x.z + m2->x.z;
    result->x.w = m1->x.w + m2->x.w;
    result->y.x = m1->y.x + m2->y.x;
    result->y.y = m1->y.y + m2->y.y;
    result->y.z = m1->y.z + m2->y.z;
    result->y.w = m1->y.w + m2->y.w;
    result->z.x = m1->z.x + m2->z.x;
    result->z.y = m1->z.y + m2->z.y;
    result->z.z = m1->z.z + m2->z.z;
    result->z.w = m1->z.w + m2->z.w;
    result->w.x = m1->w.x + m2->w.x;
    result->w.y = m1->w.y + m2->w.y;
    result->w.z = m1->w.z + m2->w.z;
    result->w.w = m1->w.w + m2->w.w;
}

void mat4_subtraction(mat4 *m1, mat4 *m2, mat4 *result) {
    result->x.x = m1->x.x - m2->x.x;
    result->x.y = m1->x.y - m2->x.y;
    result->x.z = m1->x.z - m2->x.z;
    result->x.w = m1->x.w - m2->x.w;
    result->y.x = m1->y.x - m2->y.x;
    result->y.y = m1->y.y - m2->y.y;
    result->y.z = m1->y.z - m2->y.z;
    result->y.w = m1->y.w - m2->y.w;
    result->z.x = m1->z.x - m2->z.x;
    result->z.y = m1->z.y - m2->z.y;
    result->z.z = m1->z.z - m2->z.z;
    result->z.w = m1->z.w - m2->z.w;
    result->w.x = m1->w.x - m2->w.x;
    result->w.y = m1->w.y - m2->w.y;
    result->w.z = m1->w.z - m2->w.z;
    result->w.w = m1->w.w - m2->w.w;
}

void mat4_multiplication(mat4 *m1, mat4 *m2, mat4 *result) {
    result->x.x = m1->x.x * m2->x.x + m1->y.x * m2->x.y + m1->z.x * m2->x.z + m1->w.x * m2->x.w;
    result->x.y = m1->x.y * m2->x.x + m1->y.y * m2->x.y + m1->z.y * m2->x.z + m1->w.y * m2->x.w;
    result->x.z = m1->x.z * m2->x.x + m1->y.z * m2->x.y + m1->z.z * m2->x.z + m1->w.z * m2->x.w;
    result->x.w = m1->x.w * m2->x.x + m1->y.w * m2->x.y + m1->z.w * m2->x.z + m1->w.w * m2->x.w;
    result->y.x = m1->x.x * m2->y.x + m1->y.x * m2->y.y + m1->z.x * m2->y.z + m1->w.x * m2->y.w;
    result->y.y = m1->x.y * m2->y.x + m1->y.y * m2->y.y + m1->z.y * m2->y.z + m1->w.y * m2->y.w;
    result->y.z = m1->x.z * m2->y.x + m1->y.z * m2->y.y + m1->z.z * m2->y.z + m1->w.z * m2->y.w;
    result->y.w = m1->x.w * m2->y.x + m1->y.w * m2->y.y + m1->z.w * m2->y.z + m1->w.w * m2->y.w;
    result->z.x = m1->x.x * m2->z.x + m1->y.x * m2->z.y + m1->z.x * m2->z.z + m1->w.x * m2->z.w;
    result->z.y = m1->x.y * m2->z.x + m1->y.y * m2->z.y + m1->z.y * m2->z.z + m1->w.y * m2->z.w;
    result->z.z = m1->x.z * m2->z.x + m1->y.z * m2->z.y + m1->z.z * m2->z.z + m1->w.z * m2->z.w;
    result->z.w = m1->x.w * m2->z.x + m1->y.w * m2->z.y + m1->z.w * m2->z.z + m1->w.w * m2->z.w;
    result->w.x = m1->x.x * m2->w.x + m1->y.x * m2->w.y + m1->z.x * m2->w.z + m1->w.x * m2->w.w;
    result->w.y = m1->x.y * m2->w.x + m1->y.y * m2->w.y + m1->z.y * m2->w.z + m1->w.y * m2->w.w;
    result->w.z = m1->x.z * m2->w.x + m1->y.z * m2->w.y + m1->z.z * m2->w.z + m1->w.z * m2->w.w;
    result->w.w = m1->x.w * m2->w.x + m1->y.w * m2->w.y + m1->z.w * m2->w.z + m1->w.w * m2->w.w;
}

// TODO
void v4_mat4_multiplication(vec4 *v, mat4 *m, vec4 *result) {
    result->x = v->x * m->x.x + v->x * m->x.y + v->x * m->x.z + v->x * m->x.w;
    result->y = v->y * m->y.x + v->y * m->y.y + v->y * m->y.z + v->y * m->y.w;
    result->z = v->z * m->z.x + v->z * m->z.y + v->z * m->z.z + v->z * m->z.w;
    result->w = v->w * m->w.x + v->w * m->w.y + v->w * m->w.z + v->w * m->w.w;
}

void mat4_v4_multiplication(vec4 *v, mat4 *m, vec4 *result) {
    result->x = v->x * m->x.x + v->x * m->y.x + v->x * m->z.x + v->x * m->w.x;
    result->y = v->y * m->x.y + v->y * m->y.y + v->y * m->z.y + v->y * m->w.y;
    result->z = v->z * m->x.z + v->z * m->y.z + v->z * m->z.z + v->z * m->w.z;
    result->w = v->w * m->x.w + v->w * m->y.w + v->w * m->z.w + v->w * m->w.w;
}

//~ void mat4_multiplication(mat4 *m1, mat4 *m2, mat4 *result) {
    //~ result->x.x = m1->x.x * m2->x.x + m1->y.x * m2->x.y + m1->z.x * m2->x.z + m1->w.x * m2->x.w;
    //~ result->x.y = m1->x.x * m2->y.x + m1->y.x * m2->y.y + m1->z.x * m2->y.z + m1->w.x * m2->y.w;
    //~ result->x.z = m1->x.x * m2->z.x + m1->y.x * m2->z.y + m1->z.x * m2->z.z + m1->w.x * m2->z.w;
    //~ result->x.w = m1->x.x * m2->w.x + m1->y.x * m2->w.y + m1->z.x * m2->w.z + m1->w.x * m2->w.w;
    
    //~ result->y.x = m1->x.y * m2->x.x + m1->y.y * m2->x.y + m1->z.y * m2->x.z + m1->w.y * m2->x.w;
    //~ result->y.y = m1->x.y * m2->y.x + m1->y.y * m2->y.y + m1->z.y * m2->y.z + m1->w.y * m2->y.w;
    //~ result->y.z = m1->x.y * m2->z.x + m1->y.y * m2->z.y + m1->z.y * m2->z.z + m1->w.y * m2->z.w;
    //~ result->y.w = m1->x.y * m2->w.x + m1->y.y * m2->w.y + m1->z.y * m2->w.z + m1->w.y * m2->w.w;
    
    //~ result->z.x = m1->x.z * m2->x.x + m1->y.z * m2->x.y + m1->z.z * m2->x.z + m1->w.z * m2->x.w;
    //~ result->z.y = m1->x.z * m2->y.x + m1->y.z * m2->y.y + m1->z.z * m2->y.z + m1->w.z * m2->y.w;
    //~ result->z.z = m1->x.z * m2->z.x + m1->y.z * m2->z.y + m1->z.z * m2->z.z + m1->w.z * m2->z.w;
    //~ result->z.w = m1->x.z * m2->w.x + m1->y.z * m2->w.y + m1->z.z * m2->w.z + m1->w.z * m2->w.w;
    
    //~ result->w.x = m1->x.w * m2->x.x + m1->y.w * m2->x.y + m1->z.w * m2->x.z + m1->w.w * m2->x.w;
    //~ result->w.y = m1->x.w * m2->y.x + m1->y.w * m2->y.y + m1->z.w * m2->y.z + m1->w.w * m2->y.w;
    //~ result->w.z = m1->x.w * m2->z.x + m1->y.w * m2->z.y + m1->z.w * m2->z.z + m1->w.w * m2->z.w;
    //~ result->w.w = m1->x.w * m2->w.x + m1->y.w * m2->w.y + m1->z.w * m2->w.z + m1->w.w * m2->w.w;
//~ }

void mat4_transpose(mat4 *m, mat4 *result) {
    result->x.x = m->x.x;
    result->x.y = m->y.x;
    result->x.z = m->z.x;
    result->x.w = m->w.x;
    result->y.x = m->x.y;
    result->y.y = m->y.y;
    result->y.z = m->z.y;
    result->y.w = m->w.y;
    result->z.x = m->x.z;
    result->z.y = m->y.z;
    result->z.z = m->z.z;
    result->z.w = m->w.z;
    result->w.x = m->x.w;
    result->w.y = m->y.w;
    result->w.z = m->z.w;
    result->w.w = m->w.w;
}

void mat4_minor(mat4 *m, mat4 *result) {
    result->x.x = m->y.y * m->z.z * m->w.w + m->y.z * m->z.w * m->w.y + m->y.w * m->z.y * m->w.z - m->y.w * m->z.z * m->w.y - m->y.z * m->z.y * m->w.w - m->y.y * m->z.w * m->w.z;
    result->x.y = m->y.x * m->z.z * m->w.w + m->y.z * m->z.w * m->w.x + m->y.w * m->z.x * m->w.z - m->y.w * m->z.z * m->w.x - m->y.z * m->z.x * m->w.w - m->y.x * m->z.w * m->w.z;
    result->x.z = m->y.x * m->z.y * m->w.w + m->y.y * m->z.w * m->w.x + m->y.w * m->z.x * m->w.y - m->y.w * m->z.y * m->w.x - m->y.y * m->z.x * m->w.w - m->y.x * m->z.w * m->w.y;
    result->x.w = m->y.x * m->z.y * m->w.z + m->y.y * m->z.z * m->w.x + m->y.z * m->z.x * m->w.y - m->y.z * m->z.y * m->w.x - m->y.y * m->z.x * m->w.z - m->y.x * m->z.z * m->w.y;
    result->y.x = m->x.y * m->z.z * m->w.w + m->x.z * m->z.w * m->w.y + m->x.w * m->z.y * m->w.z - m->x.w * m->z.z * m->w.y - m->x.z * m->z.y * m->w.w - m->x.y * m->z.w * m->w.z;
    result->y.y = m->x.x * m->z.z * m->w.w + m->x.z * m->z.w * m->w.x + m->x.w * m->z.x * m->w.z - m->x.w * m->z.z * m->w.x - m->x.z * m->z.x * m->w.w - m->x.x * m->z.w * m->w.z;
    result->y.z = m->x.x * m->z.y * m->w.w + m->x.y * m->z.w * m->w.x + m->x.w * m->z.x * m->w.y - m->x.w * m->z.y * m->w.x - m->x.y * m->z.x * m->w.w - m->x.x * m->z.w * m->w.y;
    result->y.w = m->x.x * m->z.y * m->w.z + m->x.y * m->z.z * m->w.x + m->x.z * m->z.x * m->w.y - m->x.z * m->z.y * m->w.x - m->x.y * m->z.x * m->w.z - m->x.x * m->z.z * m->w.y;
    result->z.x = m->x.y * m->y.z * m->w.w + m->x.z * m->y.w * m->w.y + m->x.w * m->y.y * m->w.z - m->x.w * m->y.z * m->w.y - m->x.z * m->y.y * m->w.w - m->x.y * m->y.w * m->w.z;
    result->z.y = m->x.x * m->y.z * m->w.w + m->x.z * m->y.w * m->w.x + m->x.w * m->y.x * m->w.z - m->x.w * m->y.z * m->w.x - m->x.z * m->y.x * m->w.w - m->x.x * m->y.w * m->w.z;
    result->z.z = m->x.x * m->y.y * m->w.w + m->x.y * m->y.w * m->w.x + m->x.w * m->y.x * m->w.y - m->x.w * m->y.y * m->w.x - m->x.y * m->y.x * m->w.w - m->x.x * m->y.w * m->w.y;
    result->z.w = m->x.x * m->y.y * m->w.z + m->x.y * m->y.z * m->w.x + m->x.z * m->y.x * m->w.y - m->x.z * m->y.y * m->w.x - m->x.y * m->y.x * m->w.z - m->x.x * m->y.z * m->w.y;
    result->w.x = m->y.y * m->z.z * m->x.w + m->y.z * m->z.w * m->x.y + m->y.w * m->z.y * m->x.z - m->x.w * m->y.z * m->z.y - m->x.z * m->y.y * m->z.w - m->x.y * m->y.w * m->z.z;
    result->w.y = m->y.x * m->z.z * m->x.w + m->y.z * m->z.w * m->x.x + m->y.w * m->z.x * m->x.z - m->x.w * m->y.z * m->z.x - m->x.z * m->y.x * m->z.w - m->x.x * m->y.w * m->z.z;
    result->w.z = m->y.x * m->z.y * m->x.w + m->y.y * m->z.w * m->x.x + m->y.w * m->z.x * m->x.y - m->x.w * m->y.y * m->z.x - m->x.y * m->y.x * m->z.w - m->x.x * m->y.w * m->z.y;
    result->w.w = m->y.x * m->z.y * m->x.z + m->y.y * m->z.z * m->x.x + m->y.z * m->z.x * m->x.y - m->x.z * m->y.y * m->z.x - m->x.y * m->y.x * m->z.z - m->x.x * m->y.z * m->z.y;
}

void mat4_cofactor(mat4 *m, mat4 *result) {
    result->x.x = m->x.x;
    result->x.y = -1 * m->x.y;
    result->x.z = m->x.z;
    result->x.w = -1 * m->x.w;
    result->y.x = -1 * m->y.x;
    result->y.y = m->y.y;
    result->y.z = -1 * m->y.z;
    result->y.w = m->y.w;
    result->z.x = m->z.x;
    result->z.y = -1 * m->z.y;
    result->z.z = m->z.z;
    result->z.w = -1 * m->z.w;
    result->w.x = -1 * m->w.x;
    result->w.y = m->w.y;
    result->w.z = -1 * m->w.z;
    result->w.w = m->w.w;
}

void mat4_inverse(mat4 *m, mat4 *result) {
    mat4 minor;
    mat4 cofactor;
    mat4 transpose;
    mat4_minor(m, &minor);
    mat4_cofactor(&minor, &cofactor);
    mat4_transpose(&cofactor, &transpose);
    GLfloat determinant = m->x.x * minor.x.x - m->x.y * minor.x.y + m->x.z * minor.x.z - m->x.w * minor.x.w;
    mat4_scalar_multiplication(1/determinant, &transpose, result);
    
    // Testing
    //~ printf("\n\n");
    //~ mat4_print(&minor);
    //~ printf("\n\n");
    //~ mat4_print(&cofactor);
    //~ printf("\n\n");
    //~ mat4_print(&transpose);
    //~ printf("\n");
    //~ printf("\nDet: %f\n", determinant);
}

GLfloat v4_magnitude(vec4 *v) {
    GLfloat result = 0.0;
    result += v->x * v->x;
    result += v->y * v->y;
    result += v->z * v->z;
    result = sqrt(result);
    return result;
}

GLfloat v4v4_rads_between(vec4 *v1, vec4 *v2) {
    return acos(v4_dot_product(v1, v2) / (v4_magnitude(v1) * v4_magnitude(v2)));
}

void unit_vector(vec4 *v, vec4 *result) {
    GLfloat magnitude = v4_magnitude(v);
    v4_scalar_multiplication(1.0f/magnitude, v, result);
}

void unit_vector_from_two_points(vec4 *p1, vec4 *p2, vec4 *result) {
    v4v4_subtraction(p1, p2, result);
    unit_vector(result, result);
}

void get_rotation_matrix_to_x(vec4 *v, mat4 *result) {
    GLfloat d = sqrt(v->y*v->y + v->z*v->z);
    
    result->x.x = 1;result->y.x = 0;result->z.x = 0;result->w.x = 0;
    result->x.y = 0;result->y.y = v->z/d;result->z.y = -v->y/d;result->w.y = 0;
    result->x.z = 0;result->y.z = v->y/d;result->z.z = v->z/d;result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_back_from_x(vec4 *v, mat4 *result) {
    GLfloat d = sqrt(v->y*v->y + v->z*v->z);
    
    result->x.x = 1;result->y.x = 0;result->z.x = 0;result->w.x = 0;
    result->x.y = 0;result->y.y = v->z/d;result->z.y = v->y/d;result->w.y = 0;
    result->x.z = 0;result->y.z = -v->y/d;result->z.z = v->z/d;result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_to_y(vec4 *v, mat4 *result) {
    GLfloat d = sqrt(v->y*v->y + v->z*v->z);
    
    result->x.x = d;result->y.x = 0;result->z.x = -v->x;result->w.x = 0;
    result->x.y = 0;result->y.y = 1;result->z.y = 0;result->w.y = 0;
    result->x.z = v->x;result->y.z = 0;result->z.z = d;result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_back_from_y(vec4 *v, mat4 *result) {
    GLfloat d = sqrt(v->y*v->y + v->z*v->z);
    
    result->x.x = d;result->y.x = 0;result->z.x = v->x;result->w.x = 0;
    result->x.y = 0;result->y.y = 1;result->z.y = 0;result->w.y = 0;
    result->x.z = -v->x;result->y.z = 0;result->z.z = d;result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_about_x(GLfloat theta_rads, mat4 *result) {
    result->x.x = 1;result->y.x = 0;result->z.x = 0;result->w.x = 0;
    result->x.y = 0;result->y.y = cos(theta_rads);result->z.y = -sin(theta_rads);result->w.y = 0;
    result->x.z = 0;result->y.z = sin(theta_rads);result->z.z = cos(theta_rads);result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_about_y(GLfloat theta_rads, mat4 *result) {
    result->x.x = cos(theta_rads);result->y.x = 0;result->z.x = sin(theta_rads);result->w.x = 0;
    result->x.y = 0;result->y.y = 1;result->z.y = 0;result->w.y = 0;
    result->x.z = -sin(theta_rads);result->y.z = 0;result->z.z = cos(theta_rads);result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

//~ void get_rotation_matrix_about_y_with_offset(
    //~ GLfloat theta_rads,
    //~ GLfloat x_offset, 
    //~ GLfloat y_offset, 
    //~ GLfloat z_offset, 
    //~ mat4 *result
//~ ) {
    //~ // Assumes an identity matrix!
    //~ result->w.x -= x_offset;
    //~ result->w.y -= y_offset;
    //~ result->w.z -= z_offset;
    
    //~ get_rotation_matrix_about_y(theta_rads, result);
    
    //~ // Translate back
    //~ result->w.x += x_offset;
    //~ result->w.y += y_offset;
    //~ result->w.z += z_offset;
//~ }

void get_rotation_matrix_about_z(GLfloat theta_rads, mat4 *result) {
    result->x.x = cos(theta_rads);result->y.x = -sin(theta_rads);result->z.x = 0;result->w.x = 0;
    result->x.y = sin(theta_rads);result->y.y = cos(theta_rads);result->z.y = 0;result->w.y = 0;
    result->x.z = 0;result->y.z = 0;result->z.z = 1;result->w.z = 0;
    result->x.w = 0;result->y.w = 0;result->z.w = 0;result->w.w = 1;
}

void get_rotation_matrix_about_origin(GLfloat theta_rads, vec4 *v, mat4 *result) {
    mat4 rotx;
    mat4 rotx_back;
    get_rotation_matrix_to_x(v, &rotx);
    get_rotation_matrix_back_from_x(v, &rotx_back);
    
    //~ printf("rotx:\n");
    //~ mat4_print(&rotx);
    //~ printf("\n");
    //~ printf("rotx_back:\n");
    //~ mat4_print(&rotx_back);
    //~ printf("\n");
    
    mat4 roty;
    mat4 roty_back;
    get_rotation_matrix_to_y(v, &roty);
    get_rotation_matrix_back_from_y(v, &roty_back);
    
    //~ printf("roty:\n");
    //~ mat4_print(&roty);
    //~ printf("\n");
    //~ printf("roty_back:\n");
    //~ mat4_print(&roty_back);
    //~ printf("\n");
    
    mat4 mul1;
    mat4_multiplication(&roty, &rotx, &mul1);
    
    //~ printf("rotx * roty:\n");
    //~ mat4_print(&mul1);
    //~ printf("\n");
    
    mat4 rotz;
    get_rotation_matrix_about_z(theta_rads, &rotz);
    
    //~ printf("rotz:\n");
    //~ mat4_print(&rotz);
    //~ printf("\n");
    
    mat4 mul2;
    mat4_multiplication(&rotz, &mul1, &mul2);
    
    //~ printf("rotz * roty * rotx:\n");
    //~ mat4_print(&mul2);
    //~ printf("\n");
    
    mat4 mul3;
    // Remember to rotate back!
    mat4_multiplication(&roty_back, &mul2, &mul3);
    
    //~ printf("roty_back * rotz * roty * rotx:\n");
    //~ mat4_print(&mul3);
    //~ printf("\n");
    
    mat4_multiplication(&rotx_back, &mul3, result);
    
    //~ printf("result:\n");
    //~ mat4_print(result);
    //~ printf("\n");
}

mat4 look_at(
    GLfloat eyex, GLfloat eyey, GLfloat eyez,
    GLfloat atx, GLfloat aty, GLfloat atz,
    GLfloat upx, GLfloat upy, GLfloat upz
)
{
    vec4 vup = {upx, upy, upz, 0};
    
    vec4 e = {eyex, eyey, eyez, 0};
    vec4 a = {atx, aty, atz, 0};
    
    vec4 vpn;
    vec4 n;
    v4v4_subtraction(&a, &e, &vpn);
    //~ v4v4_subtraction(&e, &a, &vpn);
    unit_vector(&vpn, &n);
    
    //~ v4_print(&vpn);
    //~ v4_print(&n);
    //~ printf("\n");
    
    vec4 u1;
    vec4 u;
    //~ v4_cross_product(&vup, &n, &u1);
    v4_cross_product(&n, &vup, &u1);
    unit_vector(&u1, &u);
    
    //~ v4_print(&u1);
    //~ v4_print(&u);
    //~ printf("\n");
    
    vec4 v1;
    vec4 v;
    //~ v4_cross_product(&n, &u, &v1);
    v4_cross_product(&u, &n, &v1);
    
    //~ v4_print(&v1);
    //~ printf("\n");
    
    // Skip normalizing v?
    unit_vector(&v1, &v);
    
    //~ v4_print(&v);
    //~ printf("\n");
    
    //~ vec4 Q0;
    //~ unit_vector(&e, &Q0);
    //~ Q0.w = 1;
    
    // Change sign
    v4_scalar_multiplication(-1.0f, &n, &n);
    //~ v4_scalar_multiplication(-1.0f, &u, &u);
    //~ v4_scalar_multiplication(-1.0f, &u1, &u1);
    //~ v4_scalar_multiplication(-1.0f, &v, &v);
    //~ v4_scalar_multiplication(-1.0f, &v1, &v1);
    
    //~ vec4 Q0;
    //~ Q0.x = -1.0f * v4_dot_product(&u, &e);
    //~ Q0.y = -1.0f * v4_dot_product(&v, &e);
    //~ Q0.z = -1.0f * v4_dot_product(&n, &e);
    //~ Q0.w = 1;
    
    mat4 initial_matrix = {
        u, v, n, {0, 0, 0, 0}
    };
    
    //~ mat4_print(&initial_matrix);
    
    //~ mat4 inverse_matrix;
    //~ mat4_inverse(&initial_matrix, &inverse_matrix);
    
    mat4 transposed_matrix;
    
    mat4_transpose(&initial_matrix, &transposed_matrix);
    
    transposed_matrix.w.x = -1.0f * v4_dot_product(&u, &e);
    transposed_matrix.w.y = -1.0f * v4_dot_product(&v, &e);
    transposed_matrix.w.z = -1.0f * v4_dot_product(&n, &e);
    transposed_matrix.w.w = 1;
    
    //~ mat4_print(&transposed_matrix);
    
    return transposed_matrix;
    //~ return inverse_matrix;
    //~ return initial_matrix;
}

mat4 identity_matrix() {
    mat4 identity_matrix = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    
    return identity_matrix;
}

mat4 empty_matrix() {
    mat4 empty_matrix = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    
    return empty_matrix;
}

mat4 ortho(
    GLfloat left, GLfloat right,
    GLfloat bottom, GLfloat top,
    GLfloat nyeh, GLfloat fyah
)
{
    mat4 ortho_matrix = identity_matrix();
    
    ortho_matrix.x.x = 2.0 / (right - left);
    ortho_matrix.y.y = 2.0 / (top - bottom);
    ortho_matrix.z.z = 2.0 / (nyeh - fyah);
    
    ortho_matrix.w.x = -((right + left)/(right - left));
    ortho_matrix.w.x = -((top + bottom)/(top - bottom));
    ortho_matrix.w.x = -((nyeh + fyah)/(nyeh - fyah));
    
    return ortho_matrix;
}

// Shear frustrum 
// Scale frustum
// Fit frustum in canonial view
mat4 frustum(
    GLfloat left, GLfloat right,
    GLfloat bottom, GLfloat top,
    GLfloat nyeh, GLfloat fyah
)
{
    mat4 frustum = empty_matrix();
    
    frustum.x.x = (-2.0f * nyeh) / (right - left);
    frustum.y.y = (-2.0f * nyeh) / (top - bottom);
    frustum.z.x = (left + right) / (right - left);
    frustum.z.y = (bottom + top) / (top - bottom);
    frustum.z.z = (nyeh + fyah) / (fyah - nyeh);
    frustum.z.w = -1.0f;
    frustum.w.z = -1.0f*(2.0f * nyeh * fyah) / (fyah - nyeh);
    
    //~ mat4 frustum = identity_matrix();
    //~ frustum.x.x = (-2 * nyeh) / (right - left);
    //~ frustum.y.y = (-2 * nyeh) / (top - bottom);
    //~ mat4 frustum = empty_matrix();
    //~ frustum.x.x = -(nyeh/right);
    //~ frustum.y.y = -(nyeh/top);
    //~ frustum.z.z = (nyeh + fyah) / (fyah - nyeh);
    //~ frustum.z.w = -1;
    //~ frustum.w.z = -((2 * nyeh * fyah)/(fyah - nyeh));
    //~ frustum.x.x = (nyeh/right);
    //~ frustum.y.y = (nyeh/top);
    //~ frustum.z.z = (nyeh + fyah) / (fyah - nyeh);
    //~ frustum.z.w = 1;
    //~ frustum.w.z = ((2 * nyeh * fyah)/(fyah - nyeh));
    return frustum;
}
