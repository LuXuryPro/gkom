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
#include "shader.h"
#include "object.h"
#include "skybox.h"
#include "celestial_body.h"
#include "SOIL/SOIL.h"
#include "depth_map.h"
#include "object.h"
#include "orbit.h"

char keys[1024];
int width = 800;
int height = 600;
int fov = 60;
GLuint program;
GLint attribute_coord;
GLint attribute_color;
GLint mvp;
GLuint vbo_cube_vertices, vbo_cube_colors , ibo_cube_elements;
Matrix4f projection;
struct Camera * camera;
struct Skybox * skybox;
GLuint sp;
GLuint s_mvp;
GLuint s_model;
GLuint s_coord;
GLuint stc;
GLuint s_t;
struct Mesh * sphere;
int mode = 0;
GLuint texture;
struct Sun * sun;
struct Earth * earth;
struct Moon * moon;
struct DepthMap * depth_map;
struct Object * plane;
int wire;
int go = 1;
float light_power = 1;
int hyper = 0;
const GLubyte* vendor;
const GLubyte* version;
const GLubyte* shading_lang_v;
const GLubyte* renderer;


void init()
{
    glPointSize(30);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    depth_map = depth_map_init();
    plane = object_plane_init();
    camera = default_Camera();
    sun = sun_init();
    earth = earth_init();
    moon = moon_init();
    skybox = init_Skybox();
}


float avg = 0;
float avgn = 0;

struct Camera light_camera;
static float f = 0.001;
void shadow_map_pass() {
    struct Vector4f translation_vector;
    translation_vector = orbit_get_position(earth->orbit_path);
    light_camera.width = depth_map->shadow_map_width;
    light_camera.height = depth_map->shadow_map_height;
    light_camera.fov = 90;
    light_camera.near_plane = 1;
    light_camera.far_plane = 20;
    light_camera.frame.position.x = translation_vector.x;
    light_camera.frame.position.y = translation_vector.y;
    light_camera.frame.position.z = translation_vector.z;
    vec4f_normalize(&translation_vector);
    float position = 4;
    light_camera.frame.position.x -= position*translation_vector.x;
    light_camera.frame.position.y -= position*translation_vector.y;
    light_camera.frame.position.z -= position*translation_vector.z;
    light_camera.frame.forward.x = -translation_vector.x;
    light_camera.frame.forward.y = -translation_vector.y;
    light_camera.frame.forward.z = -translation_vector.z;
    light_camera.frame.up.x = 0;
    light_camera.frame.up.y = 1;
    light_camera.frame.up.z = 0;
    //    camera->frame = light_camera.frame;
    depth_map_bind_for_write(depth_map);
    Matrix4f m;
    get_camera_matrix(&light_camera, m);
    earth_render(earth, m, f, moon, 1, m, light_power);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void display()
{
    shadow_map_pass();
    glViewport(0, 0, camera->width, camera->height);
    glClearColor(0, 0 , 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int start = glutGet(GLUT_ELAPSED_TIME);

    Matrix4f m;
    get_camera_matrix(camera, m);
    render_Skybox(skybox, m);
    Matrix4f plane_model;
    struct Vector4f v = {10, 0, 0, 0};
    mat4f_translate(plane_model, &v);
    render_plane_object(plane, m, depth_map->depthMap);

    sun_render(sun, m, f, light_power);
    Matrix4f light_matrix;
    earth->object->depth_texture_id = depth_map->depthMap;
    moon->object->depth_texture_id = depth_map->depthMap;
    get_camera_matrix(&light_camera, light_matrix);
    earth_render(earth, m, f, moon, 0, light_matrix, light_power);

    char hello[4096];
    sprintf(hello, "OpenGL version: %s\nShading Language Version: %s\nVendor: %s\nRenderer: %s\nFPS: %f\nSimulation Speed: %f\nLight Power: %f",
            version,
            shading_lang_v,
            vendor,
            renderer,
            avg,
            f*1000,
            light_power
           );

    render_text(0, height-18, hello, width, height);

    glFlush();
    glutSwapBuffers();

    static int frame_no = 0;
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
        camera_move_forward(camera, 0.05);
    }
    if (keys['s']) {
        camera_move_backward(camera, 0.05);
    }
    if (keys['a']) {
        camera_move_left(camera, 0.05);
    }
    if (keys['d']) {
        camera_move_right(camera, 0.05);
    }
    if (keys['j']) {
        light_power -= 0.1;
        if (light_power < 0)
            light_power = 0;
    }
    if (keys['k']) {
        light_power += 0.1;
    }

    static int ok2 = 0;
    if (keys['p']) {
        f += 0.0001;
    }
    if (keys['o']) {
        f -= 0.0001;
        if (f < 0)
            f = 0;
    }
    if (keys['p'] == 0) {
        ok2 = 0;
    }
    if (keys['q'] || keys['e']) {
        float delta= keys['e'] ? 0.5 : -0.5;
        camera_roll(camera, delta);
    }
    static int ok = 0;
    if (keys['m'] && ok == 0) {
        if (mode == 0) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            mode = 1;
        } else if (mode == 1) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            mode = 0;
        }
        ok = 1;
    }
    if (keys['m'] == 0)
        ok = 0;
}

int spin = 0;
int startx;
int starty;
static int s = 0;
static float sx;
static float sy;
void mouseFunc(int button, int state, int x, int y) {
    if (button == 4) {
        camera->fov += 1;
        if (camera->fov > 179)
            camera->fov = 179;
    } else if (button == 3) {
        camera->fov -= 1;
        if (camera->fov < 1)
            camera->fov = 1;
    } else if (button == 2) {
        if (state == GLUT_DOWN && s == 0) {
            glutWarpPointer(width/2, height/2);
            sx = width/2;
            sy = height/2;
            s = 1;
        }
        if (state == GLUT_UP) {
            s = 0;
        }
    }
}

void MotionFunc(int x, int y)
{
    static int wrapped = 0;
    if (s == 1 && !wrapped) {
        float delta = (sx - x)*0.1;
        Matrix4f m;
        mat4f_rot(m, &camera->frame.forward, delta);
        mat4f_vec_mul(m, &camera->frame.up);
        vec4f_normalize(&camera->frame.up);
        sx = width/2;
        sy = height/2;
        glutWarpPointer(width/2, height/2);
        wrapped = 1;
    } else
        wrapped = 0;
}
void passiveMotionFunc(int x, int y)
{
    static int wrapped = 0;
    if (!wrapped && s == 0) {
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
    } else
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
        glViewport( 0, 0, w, h );
        camera->width = w;
        camera->height = h;
    }
}
int main(int argc, char** argv)
{
    glutInit( &argc, argv );

    glutInitDisplayMode( GLUT_DOUBLE |
            GLUT_RGBA |
            GLUT_DEPTH |
            GLUT_MULTISAMPLE
            );

    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( width , height );

    glutCreateWindow( "OpenGL Gravity" );
    glutFullScreen();

    glutSetCursor(GLUT_CURSOR_NONE);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutIdleFunc( display );
    glutMouseFunc( mouseFunc );
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(processNormalKeysUp);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMotionFunc(MotionFunc);

    //glewExperimental = GL_TRUE ;
    version = glGetString(GL_VERSION);
    vendor = glGetString(GL_VENDOR);
    renderer = glGetString(GL_RENDERER);
    shading_lang_v = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint status;
    if ((status = glewInit()) != GLEW_OK)
    {
        printf("%d\n", status);
        return -1;
    }
    init();
    glutMainLoop();

    return 0;
}
