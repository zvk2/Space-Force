#include "shapes.h"

vec4 v4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    vec4 result = { x, y, z, w };
    return result;
}

vec4 *sphere(GLfloat radius, int x, int y, int *num_vertices) 
{
    int lats = (int)180/x;
    int longs = (int)360/y;
    int num_points = lats * longs + 2;

    // my malloc is wrong but whatever
    vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * num_points * num_points);

    vec4 points[num_points];
        
    GLfloat theta;
    GLfloat phi;    
        
    points[0].x = 0;
    points[0].y = radius;
    points[0].z = 0;
    points[0].w = 1;

    int index = 1;

    GLfloat rad_multiply = M_PI/180.0;

    for (theta=x; theta <= 180; theta += x) {
        GLfloat current_radius = sin(theta * rad_multiply) * radius;
        GLfloat current_y_loc = cos(theta * rad_multiply) * radius;
        // Creating points in commented order puts faces backwards
        // for (phi=0; phi < 360; phi += y) {
        for (phi=360; phi > 0; phi -= y) {
            points[index].x = cos(phi * rad_multiply) * current_radius;
            points[index].y = current_y_loc;
            points[index].z = sin(phi * rad_multiply) * current_radius;
            points[index].w = 1.0;
            index++;
        }
    }

    points[num_points - 1].x = 0;
    points[num_points - 1].y = -radius;
    points[num_points - 1].z = 0;
    points[num_points - 1].w = 1.0;

    // MAKE VERTICES
    index = 0;
    int i;
    for (i=1; i<longs+1; i++) 
    {
        vertices[index] = points[0];
        vertices[index+1] = points[i];
        // Wrap around
        if (i == longs)
            vertices[index+2] = points[1];
        else
            vertices[index+2] = points[i+1];
              
        index += 3;
    }

    int current_lat;
    int current_long;
    for (current_lat=0; current_lat<lats-1; current_lat++) {
        for (current_long=1; current_long<longs+1; current_long++) {        
            vertices[index] = points[current_long+longs*current_lat];
            vertices[index+1] = points[current_long+longs*(current_lat+1)];
            // Wrap around
            if (current_long % longs == 0) {
                vertices[index+2] = points[1 + longs*(current_lat+1)];
            }
            else {
                vertices[index+2] = points[current_long+1+longs*(current_lat+1)];
            }
            
            vertices[index+3] = points[current_long+longs*current_lat];
            // Wrap around
            if (current_long % longs == 0) {
                vertices[index+5] = points[1 + longs*(current_lat)];
            }
            else {
                vertices[index+5] = points[1+ current_long + longs*(current_lat)];
            }
            // Wrap around
            if (current_long % longs == 0) {
                vertices[index+4] = points[1 + longs*(current_lat+1)];
            }
            else {
                vertices[index+4] = points[current_long+1+longs*(current_lat+1)];
            }
            
            index += 6;
        }
    }

    for (i=num_points-longs; i<num_points-1; i++) {
        vertices[index] = points[num_points-1];
        vertices[index+1] = points[i];
        // Wrap around
        if (i == num_points-2) 
            vertices[index+2] = points[num_points-longs];
        else
            vertices[index+2] = points[i+1];
        
        index += 3;
    }

    // printf("index: %d\n", index);
    *num_vertices = index;
    return vertices;
}

vec4 *cube(int *num_vertices)
{
    *num_vertices = 36;

    vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * (*num_vertices));

    // Face 1
    vertices[1] = v4(1.0, 1.0, -1.0, 1.0);
    vertices[0] = v4(1.0, -1.0, -1.0, 1.0);
    vertices[2] = v4(-1.0, 1.0, -1.0, 1.0);
    vertices[3] = v4(-1.0, 1.0, -1.0, 1.0);
    vertices[4] = v4(-1.0, -1.0, -1.0, 1.0);
    vertices[5] = v4(1.0, -1.0, -1.0, 1.0);
    // Face 2
    vertices[7] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[6] = v4(1.0, -1.0, 1.0, 1.0);
    vertices[8] = v4(1.0, 1.0, 1.0, 1.0);
    vertices[9] = v4(-1.0, -1.0, 1.0, 1.0);
    vertices[10] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[11] = v4(1.0, -1.0, 1.0, 1.0);
    // Face 3
    vertices[13] = v4(1.0, -1.0, 1.0, 1.0);
    vertices[12] = v4(1.0, -1.0, -1.0, 1.0);
    vertices[14] = v4(1.0, 1.0, 1.0, 1.0);
    vertices[15] = v4(1.0, 1.0, 1.0, 1.0);
    vertices[16] = v4(1.0, 1.0, -1.0, 1.0);
    vertices[17] = v4(1.0, -1.0, -1.0, 1.0);
    // Face 4
    vertices[19] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[18] = v4(-1.0, -1.0, -1.0, 1.0);
    vertices[20] = v4(-1.0, -1.0, 1.0, 1.0);
    vertices[21] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[22] = v4(-1.0, -1.0, -1.0, 1.0);
    vertices[23] = v4(-1.0, 1.0, -1.0, 1.0);
    // Face 5
    vertices[25] = v4(1.0, 1.0, 1.0, 1.0);
    vertices[24] = v4(1.0, 1.0, -1.0, 1.0);
    vertices[26] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[27] = v4(-1.0, 1.0, 1.0, 1.0);
    vertices[28] = v4(-1.0, 1.0, -1.0, 1.0);
    vertices[29] = v4(1.0, 1.0, -1.0, 1.0);
    // Face 6
    vertices[31] = v4(-1.0, -1.0, 1.0, 1.0);
    vertices[30] = v4(1.0, -1.0, -1.0, 1.0);
    vertices[32] = v4(1.0, -1.0, 1.0, 1.0);
    vertices[33] = v4(-1.0, -1.0, 1.0, 1.0);
    vertices[34] = v4(1.0, -1.0, -1.0, 1.0);
    vertices[35] = v4(-1.0, -1.0, -1.0, 1.0);

    return vertices;
}

vec4 *circle(int *num_vertices)
{
    float theta, theta_r, theta10_r;
    int index = 0;

    *num_vertices = 108;
    vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * 108);

    for (theta = 0; theta <= 350; theta = theta + 10)
    {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;

        vertices[index] = v4(0.0, 0.0, 0.0, 1.0);
        vertices[index + 1] = v4(cos(theta_r), sin(theta_r), 0.0, 1.0);
        vertices[index + 2] = v4(cos(theta10_r), sin(theta10_r), 0.0, 1.0);
        index += 3;
    }

    return vertices;
}

vec4 *cone(int *num_vertices)
{
    float theta, theta_r, theta10_r;
    int index = 0;

    *num_vertices = 216;
    int height = 1;
    vec4 *vertices = (vec4 *)malloc(sizeof(vec4) * 216);

    // draw base
    for (theta = 0; theta <= 350; theta = theta + 10)
    {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;

        vertices[index] = v4(0.0, 0.0, 0.0, 1.0);
        vertices[index + 1] = v4(0.5*cos(theta_r), 0.0, 0.5*sin(theta_r), 1.0);
        vertices[index + 2] = v4(0.5*cos(theta10_r), 0.0, 0.5*sin(theta10_r), 1.0);
        index += 3;
    }

    // draw sides
    for (theta = 0; theta <= 350; theta = theta + 10)
    {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;

        vertices[index] = v4(0.0, 0.8, 0.0, 1.0);
        vertices[index + 1] = v4(0.5*cos(theta_r), 0.0, 0.5*sin(theta_r), 1.0);
        vertices[index + 2] = v4(0.5*cos(theta10_r), 0.0, 0.5*sin(theta10_r), 1.0);
        index += 3;
    }

    return vertices;
}