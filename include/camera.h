#ifndef CAMERA_H
#define CAMERA_H
#include "math3d.h"

struct Camera {
    struct FrameOfReference frame;
    float fov; /*Field of view in degrees*/
    int width; /*Width of rendered frame*/
    int height; /*Height of rendered frame*/
    float far_plane;
    float near_plane;
};

struct Camera * init_camera(struct FrameOfReference * frame, float fov);
struct Camera * default_Camera();
void get_camera_matrix(struct Camera * camera, Matrix4f matrix);
void camera_move_forward(struct Camera * camera, float offset);
void camera_move_backward(struct Camera * camera, float offset);
void camera_move_right(struct Camera * camera, float offset);
void camera_move_left(struct Camera * camera, float offset);
void camera_roll(struct Camera * camera, float offset);
void destroy_camera(struct Camera * camera);


#endif
