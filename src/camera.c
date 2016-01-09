#include "camera.h"
#include "stdlib.h"
#include <GL/gl.h>
#include <GL/glu.h>

struct Camera * init_camera(struct FrameOfReference * frame, float fov)
{
    struct Camera * camera = (struct Camera *) malloc(sizeof(struct Camera));
    camera->frame.position = frame->position;
    camera->frame.up = frame->up;
    camera->frame.forward = frame->forward;
    camera->fov = fov;
    return camera;
}

struct Camera * default_Camera()
{
    struct Camera * camera = (struct Camera *) malloc(sizeof(struct Camera));
    camera->fov = 45;
    camera->frame.forward.x = 0;
    camera->frame.forward.y = 0;
    camera->frame.forward.z = 1;
    camera->frame.up.x = 0;
    camera->frame.up.y = 1;
    camera->frame.up.z = 0;
    camera->frame.position.x = 0;
    camera->frame.position.y = 0;
    camera->frame.position.z = 5;
    camera->width = 800;
    camera->height = 600;
    return camera;
}

void camera_move_forward(struct Camera * camera, float offset)
{
    vec4f_normalize(&camera->frame.forward);
    struct Vector4f f = camera->frame.forward;
    /*TODO fix this flip in math3d.c -> frameofreference_to_mat4f*/
    vec4f_flip(&f);
    vec4f_scale(&f, offset);
    vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
}

void camera_move_backward(struct Camera * camera, float offset)
{
    vec4f_normalize(&camera->frame.forward);
    struct Vector4f f = camera->frame.forward;
    vec4f_scale(&f, offset);
    vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
}


void camera_move_right(struct Camera * camera, float offset)
{
    struct Vector4f f;
    vec4f_cross(&camera->frame.forward, &camera->frame.up, &f);
    vec4f_normalize(&f);
    vec4f_scale(&f, offset);
    vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
}

void camera_move_left(struct Camera * camera, float offset)
{
    struct Vector4f f;
    vec4f_cross(&camera->frame.forward, &camera->frame.up, &f);
    vec4f_normalize(&f);
    vec4f_flip(&f);
    vec4f_scale(&f, offset);
    vec4f_sum(&camera->frame.position, &f, &camera->frame.position);
}

void camera_roll(struct Camera * camera, float offset)
{
    Matrix4f m;
    mat4f_rot(m, &camera->frame.forward, offset);
    mat4f_vec_mul(m, &camera->frame.up);
    vec4f_normalize(&camera->frame.up);
}
void get_camera_matrix(struct Camera * camera, Matrix4f matrix)
{
    Matrix4f v;
    frameofreference_to_mat4f(&camera->frame, v);
    Matrix4f p;
    mat4f_init_perspective(p, RADIANS(camera->fov),
            (float)camera->width/(float)camera->height,
            0.01, 100);
    mat4f_mul(p, v, matrix);
}

void destroy_camera(struct Camera * camera)
{

}
