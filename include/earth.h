#ifndef EARTH_H
#define EARTH_H
#include "light.h"
#include "object.h"

struct Earth {
    struct Object * object;
};

struct Earth * earth_init();
void earth_render(struct Earth* earth, Matrix4f pv, float frame);

#endif
