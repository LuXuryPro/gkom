#include "camera.h"
#include "stdlib.h"
#include <GL/gl.h>

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
    camera->fov=60;
    camera->frame.forward.x = 0;
    camera->frame.forward.y = 0;
    camera->frame.forward.z = -1;
    camera->frame.up.x = 0;
    camera->frame.up.y = 1;
    camera->frame.up.z = 0;
    camera->frame.position.x = 0;
    camera->frame.position.y = 0;
    camera->frame.position.z = 0;
}

/*Depricated*/
void use_camera(struct Camera * camera)
{
    Matrix4f m;
    frameofreference_to_mat4f(&camera->frame, m);
    glLoadIdentity();
    glMultMatrixf(m);

}

void destroy_camera(struct Camera * camera)
{

}
