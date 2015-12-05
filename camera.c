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
