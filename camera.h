#ifndef CAMERA_H
#define CAMERA_H
#include "math3d.h"

struct Camera {
    struct FrameOfReference frame;
    float fov;
};

struct Camera * init_camera(struct FrameOfReference * frame, float fov);
struct Camera * default_Camera();
void use_camera(struct Camera * camera);
void destroy_camera(struct Camera * camera);



#endif
