#ifndef GRAVITY_H
#define GRAVITY_H
#include "math3d.h"


struct Planet
{
    struct Vector4f position;
    float radius;
    float mass;
};

struct Engine
{
    struct FrameOfReference camera;
};

#endif
