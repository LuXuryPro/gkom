/*
*/

#ifdef _WIN32
#include <windows.h>
#include "glut.h"
#endif
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string.h>
#include "gravity.h"
#include "math3d.h"
#include "camera.h"
#include "gui.h"

int timeSinceStart;
float speed;
float xr;
float yr;
int width = 800;
int height = 600;
float fov = 60;

struct Camera * camera;
char keys[1024];
Matrix4f lightProjectionMatrix;
Matrix4f lightViewMatrix;



#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;
GLuint shadowMapTexture;
GLuint shadowMapSize = 1024;


void makeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
}


GLUquadricObj *qObj;
void init()
{
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1, 50.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
    glPopMatrix();

    glLoadIdentity();
    gluLookAt( 0, 3, 6,
    0.0f, 0.0f, -5.0f,
    0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
    glPopMatrix();


    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Enable shadow comparison
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    //Shadow comparison should be true (ie not in shadow) if r<=texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    //Shadow comparison should generate an INTENSITY result
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize,
        shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);


    struct FrameOfReference frame;
    frame.up.x = 0;
    frame.up.y = 1;
    frame.up.z = 0;
    frame.forward.x = 0;
    frame.forward.y = 0;
    frame.forward.z = 1;
    frame.position.z = 5;

    camera = init_camera(&frame, 45);

    FILE *f = fopen("2_no_clouds_8k.rgb","rb");
    GLubyte * t = (GLubyte*)malloc(1024*1024*3);
    fread(t, 1024*1024*3, 1, f);
    fclose(f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, t);

    GLfloat mat_ambient[]    = { 1.0, 1.0,  1.0, 1.0 };
    GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    GLfloat lm_ambient[]     = { 0.2, 0.2,  0.2, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialf( GL_FRONT, GL_SHININESS, 50.0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lm_ambient );

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE); // automatyczna normalizacja wektorow normalnych
    glShadeModel(GL_SMOOTH); //cieniowanie interpalarne nie plaskie

    qObj = gluNewQuadric();
    gluQuadricNormals(qObj, GLU_SMOOTH);
    gluQuadricTexture(qObj, GL_TRUE);
}

void displayObjects(int frame_no)
{
    GLfloat torus_diffuse[]  = { 0.7, 0.7, 0.0, 1.0 };
    GLfloat cube_diffuse[]   = { 0.0, 0.7, 0.7, 1.0 };
    GLfloat octa_diffuse[]   = { 0.7, 0.4, 0.4, 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();
    glRotatef(frame_no, 0.0, 1.0, 0.0);
    glTranslatef( -0.80, 0.35, 0.0 );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, torus_diffuse );
    glutSolidTorus( 0.275, 0.85, 10, 10 );
    glPopMatrix();

    glPushMatrix();
    glRotatef(-frame_no, 0.0, 1.0, 0.0);
    glTranslatef( -0.75, -0.50, 0.0 );
    glRotatef( 45.0, 0.0, 0.0, 1.1 );
    glRotatef( 45.0, 1.0, 0.0, 0.0 );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, cube_diffuse );
    glutSolidCube( 1.5/2 );
    glPopMatrix();
    glPushMatrix();
    glTranslatef( 0, 0, -5 );
    glRotatef((glutGet(GLUT_ELAPSED_TIME)/10) % 360, 0.0, 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, cube_diffuse );
    gluSphere( qObj, 1.0, 100, 100 );
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0, 0, 0 );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, octa_diffuse );
    glutSolidTeapot( 1.0/2 );
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);
    GLfloat vertices[] = {
        0, 1, 0,
        0, -1, 0,
        1, 0, 0,
        -1,0, 0,
        0.71, 0.71, 0,
        -0.71, -0.71, 0,
        -0.71, 0.71, 0,
        0.71, -0.71, 0,
        0, 0, 0,
    };
    GLubyte indices[] = {0,8,4,
        4,8,2,
        2,8,7,
        7,8,1,
        1,8,5,
        5,8,3,
        3,8,6,
        6,8,0
    };

    GLfloat map[] = {
        0.5, 1,
        0.5, 0,
        1, 0.5,
        0, 0.5,
        0.75, 0.75,
        0.25, 0.25,
        0.25, 0.75,
        0.75, 0.25,
        0.5, 0.5
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, map);
    glPushMatrix();
    glTranslatef(0, 0,-1);
    glRotatef(frame_no, 0, 0, 1);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_BYTE, indices);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 1);
    glRotatef(frame_no, 0, 0, 1);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_BYTE, indices);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glTranslatef(0,0,-10);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f, -0.5f );
    glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -2, 0);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, torus_diffuse );
    glBegin(GL_QUADS);
    glVertex3f(  10, 0, 10 );
    glVertex3f(  -10, 0, 10 );
    glVertex3f(  -10,  0, -10 );
    glVertex3f(  10,  0, -10 );
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

float avg = 0;
float avgn = 0;

void render_shadow_map()
{
    //First pass - from light's point of view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(lightProjectionMatrix);

    //Use viewport the same size as the shadow map
    glViewport(0, 0, shadowMapSize, shadowMapSize);
    //Draw back faces into the shadow map
    glCullFace(GL_FRONT);
    //Disable color writes, and use flat shading for speed
    glShadeModel(GL_FLAT);
    glColorMask(1, 1, 1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(lightViewMatrix);
    displayObjects(0);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //Read the depth buffer into the shadow map texture
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

    //restore states
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glColorMask(1, 1, 1, 1);
}


void render_from_camera() {
    //2nd pass - Draw from camera's point of view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(camera->fov,(float)width/(float)height, 0.01, 1000);


    GLfloat mat_ambient[]    = { 0.2, 0.2,  0.2, 1 };
    GLfloat black[]    = { 0, 0,  0, 1 };
    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position );
    glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, black);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    use_camera(camera);
    displayObjects(0);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void render_shadow_texture()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(camera->fov,(float)width/(float)height, 0.01, 1000);
    GLfloat mat_ambient[]    = { 1, 1,  1, 1 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_ambient);
    //Calculate texture matrix for projection
    //This matrix takes us from eye space to the light's clip space
    //It is postmultiplied by the inverse of the current view matrix when specifying texgen
    Matrix4f biasMatrix = {0};
    biasMatrix[0] = 0.5;
    biasMatrix[5] = 0.5;
    biasMatrix[10] = 0.5;
    biasMatrix[12] = 0.5;
    biasMatrix[13] = 0.5;
    biasMatrix[14] = 0.5;
    biasMatrix[15] = 1;

    Matrix4f textureMatrix;
    Matrix4f shadowMatrix;
    mat4f_mul(biasMatrix, lightProjectionMatrix , textureMatrix);
    mat4f_mul(textureMatrix, lightViewMatrix, shadowMatrix);


    float row[4][4];
    int i;
    for (i = 0; i < 4; i++) {
        int j;
        for (j = 0; j < 4; j++) {
            row[i][j] = shadowMatrix[j*4 + i];
        }
    }

    //Set up texture coordinate generation.
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, row[0]);
    glEnable(GL_TEXTURE_GEN_S);

    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE, row[1]);
    glEnable(GL_TEXTURE_GEN_T);

    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, row[2]);
    glEnable(GL_TEXTURE_GEN_R);

    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, row[3]);
    glEnable(GL_TEXTURE_GEN_Q);

    //Bind & enable shadow map texture
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glEnable(GL_TEXTURE_2D);

    glAlphaFunc(GL_GEQUAL, 0.99f);
    glEnable(GL_ALPHA_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    use_camera(camera);
    displayObjects(0);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //Disable textures and texgen
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);

    //Restore other states
    glDisable(GL_ALPHA_TEST);
}
void display()
{
    static int frame_no = 0;
    int start = glutGet(GLUT_ELAPSED_TIME);

    render_shadow_map();
    //render_from_camera();
    render_shadow_texture();

    char hello[4096];
    sprintf ( hello, "FPS: %f\nRadek\n123", avg);
    render_text(0, height-18, hello, width, height);

    glFlush();
    glutSwapBuffers(); //bufory zalezne od systemu

    int end  = glutGet(GLUT_ELAPSED_TIME);
    int delta = end - start;
    if (delta == 0)
        delta = 1;
    avgn += (1000/((float)delta*100));
    frame_no ++;
    if (frame_no > 100) {
        avg = avgn;
        avgn = 0;
        frame_no = 0;
    }
    if (keys['w']) {
        vec4f_normalize(&camera->frame.forward);
        struct Vector4f f = camera->frame.forward;
        vec4f_flip(&f);
        vec4f_scale(&f, 0.05);
        vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
    }
    if (keys['s']) {
        vec4f_normalize(&camera->frame.forward);
        struct Vector4f f = camera->frame.forward;
        vec4f_scale(&f, 0.05);
        vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
    }
    if (keys['a']) {
        struct Vector4f f;
        vec4f_cross(&camera->frame.forward, &camera->frame.up, &f);
        vec4f_normalize(&f);
        vec4f_flip(&f);
        vec4f_scale(&f, 0.05);
        vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
    }
    if (keys['d']) {
        struct Vector4f f;
        vec4f_cross(&camera->frame.forward, &camera->frame.up, &f);
        vec4f_normalize(&f);
        vec4f_scale(&f, 0.05);
        vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
    }
    if (keys['q'] || keys['e']) {
        float delta= keys['e'] ? 0.5 : -0.5;
        Matrix4f m;
        mat4f_rot(m, &camera->frame.forward, delta);
        mat4f_vec_mul(m, &camera->frame.up);
        vec4f_normalize(&camera->frame.up);
    }
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == 4) {
        camera->fov += 1;
        if (camera->fov > 179)
            camera->fov = 179;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(camera->fov,(float)width/(float)height, 0.01, 1000);
        glMatrixMode( GL_MODELVIEW ); //GL_MODLEVIEW, GL_PROJECTION
    }
    else if (button == 3) {
        camera->fov -= 1;
        if (camera->fov < 1)
            camera->fov = 1;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(camera->fov,(float)width/(float)height, 0.01, 1000);
        glMatrixMode( GL_MODELVIEW ); //GL_MODLEVIEW, GL_PROJECTION
    }
}

void passiveMotionFunc(int x, int y)
{
    static int wrapped = 0;
    if (!wrapped) {
        wrapped = 1;
        int middlex = width/2;
        int middley = height/2;
        float deltax = (x - middlex)*0.1;
        float deltay = (middley - y)*0.1;
        Matrix4f m;
        mat4f_rot(m, &camera->frame.up, deltax);
        mat4f_vec_mul(m, &camera->frame.forward);

        struct Vector4f right;
        vec4f_cross(&camera->frame.up, &camera->frame.forward, &right);

        mat4f_rot(m, &right, deltay);
        mat4f_vec_mul(m, &camera->frame.forward);

        vec4f_cross(&camera->frame.forward, &right, &camera->frame.up);
        vec4f_normalize(&camera->frame.up);

        glutWarpPointer(width/2, height/2);
    }
    else
        wrapped = 0;
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    if (key == 27)
        exit(0);
    keys[key] = 1;
}
void processNormalKeysUp(unsigned char key, int xx, int yy) {
    keys[key] = 0;
}
void reshape(GLsizei w, GLsizei h)
{
    if( h > 0 && w > 0 ) {
        width = w;
        height = h;
        glViewport( 0, 0, w, h );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective(fov,(float)w/(float)h, 0.01, 1000);
        return;
    }
}

int main(int argc, char** argv)
{
    glutInit( &argc, argv );

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( width , height );

    glutCreateWindow( "OpenGL" );

    glutSetCursor(GLUT_CURSOR_NONE);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutIdleFunc( display );
    glutMouseFunc( mouseFunc );
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(processNormalKeysUp);
    glutPassiveMotionFunc(passiveMotionFunc);

    glewExperimental = GL_TRUE ;
    GLint status;
    if ((status = glewInit()) != GLEW_OK)
    {
        printf("%d\n", status);
        return -1;
    }
    GLuint program = compile_program("shader.vert", "shader.frag");
    init();
    glutMainLoop();

    return 0;
}
