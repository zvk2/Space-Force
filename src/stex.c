#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "shader.h"
#include "matrix.h"
#include "shapes.h"
#include "glutFuncs.h"

#include <stdio.h>
#include <time.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// sphere is 4797. cube is 36. shadows are sum of every object
vec4 vertices[4797];
vec4 colors[4797];

int num_vertices = 6, user_choice;

GLfloat eyex, eyey, eyez, atx, aty, atz;

GLuint look_location, ctm_location;
mat4 view, ctm;

vec4 *genRandomTriangleColors(int num_vertices)
{
    GLfloat r, g, b;
    int index = 0, i;

    srand(time(0));

    vec4 *colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);

    for (i = 0; i < num_vertices / 3; i++)
    {
        r = rand() / (float)RAND_MAX;
        g = rand() / (float)RAND_MAX;
        b = rand() / (float)RAND_MAX;

        colors[index] = v4(r, g, b, 1.0);
        colors[index + 1] = v4(r, g, b, 1.0);
        colors[index + 2] = v4(r, g, b, 1.0);
        index += 3;
    }

    return colors;
}

void init(void)
{
    glFrontFace(GL_CCW);

    // construct new shaders
    Shader earth("earth");
    GLuint earth_prog = earth.getProgram();
    Shader polys("polys");
    GLuint polys_prog = polys.getProgram();
    GLuint program = 0;

    // set program based on which shader we want to see
    if (user_choice == 2)
    {
        glUseProgram(polys_prog);
        program = polys_prog;
    }
    else {
        glUseProgram(earth_prog);
        program = earth_prog;
    }

    /******** geometry bureaucracy ********/
    int num_cubes = 1;
    vec4 *cube_verts = cube(&num_vertices);
    vec4 *cube_colors = genRandomTriangleColors(num_vertices * num_cubes);

    vec4 *sphere_verts = sphere(0.8, 9, 9, &num_vertices);
    vec4 *sphere_colors = genRandomTriangleColors(num_vertices);

    int i, gi = 0;
    /******** texture bureaucracy ********/
    int width = 256;
    int height = 256;
    GLubyte posx[width][height][3];
    GLubyte posy[width][height][3];
    GLubyte posz[width][height][3];
    GLubyte negx[width][height][3];
    GLubyte negy[width][height][3];
    GLubyte negz[width][height][3];

    FILE *fp;
    fp = fopen("src/posx.raw", "r");
    fread(posx, width * height * 3, 1, fp);
    fclose(fp);
    fp = fopen("src/posy.raw", "r");
    fread(posy, width * height * 3, 1, fp);
    fclose(fp);
    fp = fopen("src/posz.raw", "r");
    fread(posz, width * height * 3, 1, fp);
    fclose(fp);
    fp = fopen("src/negx.raw", "r");
    fread(negx, width * height * 3, 1, fp);
    fclose(fp);
    fp = fopen("src/negy.raw", "r");
    fread(negy, width * height * 3, 1, fp);
    fclose(fp);
    fp = fopen("src/negz.raw", "r");
    fread(negz, width * height * 3, 1, fp);
    fclose(fp);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mytex[0]);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0); 
    //Define all 6 faces
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, posx);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, negx);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, posy);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, negy);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, posz);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, negz);

    /******** put shapes into final vertex array ********/
    // for (i = 0; i < 36; i++)
    // {
    //     vertices[gi] = mat_vec_mult(cubemat, cube_verts[i]);
    //     colors[gi] = cube_colors[gi];
    //     gi++;
    // }

    // putting our sphere into our global verts array
    for (i = 0; i < num_vertices; i++)
    {
        // vertices[gi] = sphere_verts[i];
        vertices[gi] = sphere_verts[i];
        colors[gi] = sphere_colors[gi];
        gi++;
    }

    for (i = 0; i < num_vertices; i++)
    {
        // vec4 temp = v_sub(vertices[i], v4(0, 0, 0, 0));
        colors[i] = unit_vector(vertices[i]);
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int size_vertices = sizeof(vertices)/sizeof(vec4);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * size_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * size_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * size_vertices, sizeof(vec4) * size_vertices, colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * size_vertices));

    // IN A MORE ROBUST AND CLEAN SHADER CLASS, SHOULD PROBABLY WRAP THIS INSIDE
    // vector<> of uniform locations??
    look_location = glGetUniformLocation(program, "look");
    ctm_location = glGetUniformLocation(program, "ctm");

    GLuint cubeMap_location = glGetUniformLocation(program, "cubeMap");
    glUniform1i(cubeMap_location, 0);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDepthRange(1,0);
}

void get_at()
{
    printf("Please enter an eye point: ");
    scanf("%f, %f, %f", &eyex, &eyey, &eyez);

    printf("Please enter an at point: ");
    scanf("%f, %f, %f", &atx, &aty, &atz);

    printf("%f, %f, %f, %f, %f, %f\n", eyex, eyey, eyez, atx, aty, atz);
}

int main(int argc, char **argv)
{
    eyex = 0;   eyey = 0.1; eyez = 0.1;
    atx = 0;    aty = 0;    atz = 0;
    // get_at();

    view = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);

    printf("Would you like to see Earth or normals?\n");
    printf("Please enter the number of your selection:\n");
    printf("1. Earth\n");
    printf("2. Normals\n");
    printf("Default is Earth.\n");
    printf("User choice: ");
    printf("\n");
    scanf("%d", &user_choice);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Sphere Proof of Concept");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
