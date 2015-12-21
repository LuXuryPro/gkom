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
    camera->frame.forward.z = -1;
    camera->frame.up.x = 0;
    camera->frame.up.y = 1;
    camera->frame.up.z = 0;
    camera->frame.position.x = 0;
    camera->frame.position.y = 0;
    camera->frame.position.z = -10;
    camera->width = 800;
    camera->height = 600;
    return camera;
}

/*Depricated*/
void get_camera_matrix(struct Camera * camera, Matrix4f matrix)
{
    Matrix4f v;
    frameofreference_to_mat4f(&camera->frame, v);
    Matrix4f p;
    mat4f_init_perspective(p, RADIANS(camera->fov),
            (float)camera->width/(float)camera->height,
            0.01, 5000);
    mat4f_mul(p, v, matrix);
}

void destroy_camera(struct Camera * camera)
{

}
