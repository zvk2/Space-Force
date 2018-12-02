#include "glutFuncs.h"

GLfloat radians = 0.0025;
GLfloat x_value = 0;
GLfloat y_value = 0.2;
int isGoingRight = 1;
int isGoingUp = 0;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    // shapes
    // glUniformMatrix4fv(look_location, 1, GL_FALSE, (GLfloat *) &view);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // glUniformMatrix4fv(look_location, 1, GL_FALSE, (GLfloat *) &view);
    // glDrawArrays(GL_TRIANGLES, 36, 4797);

    // shadows
    // glUniformMatrix4fv(look_location, 1, GL_FALSE, (GLfloat *) &view);
    // glDrawArrays(GL_TRIANGLES, 4833, 36);

    // glUniformMatrix4fv(look_location, 1, GL_FALSE, (GLfloat *) &view);
    // glDrawArrays(GL_TRIANGLES, 4869, 4797);

    // not particularly efficient. i think in the end we would be passing radians into the
    // shader and doing that kind of math inside of the GPU. but for spofcon purposes, who
    // cares
    ctm = m_mult(m_mult(scale_matrix(0.5, 0.5, 0.5), y_rot_matrix(radians)), translation_matrix(-0.25, 0.25, -0.1));
    radians += 0.001;
    if (radians > 360)
        radians = 0;
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glUniformMatrix4fv(look_location, 1, GL_FALSE, (GLfloat *) &view);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    //printf("key: %i\n", key);
    if(key == 'q')
    	exit(0);

    // this is broken lol
    if(key == 'd')
    {
        GLfloat eyexr = cos((eyex+0.001)*M_PI/180.0);
        GLfloat eyezr = sin((eyez+0.001)*M_PI/180.0);
        eyex += 0.001;
        eyez += 0.001;
        view = look_at(eyexr, eyey, eyezr, atx, aty, atz, 0.0, 1.0, 0.0);
        printf("t: %f, %f\n", eyex, eyez);
        printf("r: %f, %f\n", eyexr, eyezr);
    }
    if(key == 'a')
    {
        GLfloat eyexr = cos((eyex-0.001)*M_PI/180.0);
        GLfloat eyezr = sin((eyez-0.001)*M_PI/180.0);
        eyex -= 0.001;
        eyez -= 0.001;
        view = look_at(eyexr, eyey, eyezr, atx, aty, atz, 0, 1, 0);
        printf("t: %f, %f\n", eyex, eyez);
        printf("r: %f, %f\n", eyexr, eyezr);
    }

    glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

void idle(void)
{
    glutPostRedisplay();
}