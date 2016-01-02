#ifndef SUN_H
#define SUN_H
#include "light.h"
#include "object.h"

struct Sun {
    struct Light light;
    float rotation_angle;
    struct Object * object;
};

struct Sun * sun_init();
void sun_render(struct Sun * sun, Matrix4f pv, float frame);

#endif
