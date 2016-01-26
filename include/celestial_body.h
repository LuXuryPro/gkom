#ifndef CELESTIAL_BODY
#define CELESTIAL_BODY
#include "light.h"
#include "object.h"
#include "orbit.h"

struct Sun {
    struct Light light;
    float rotation_angle;
    struct Object * object;
};

struct Sun * sun_init();
void sun_render(struct Sun * sun,
        Matrix4f pv,
        float frame,
        float light_power);

struct Moon {
    struct Object * object;
    struct Object * orbit;
    struct Orbit * orbit_path;
    float rotation;
};

struct Moon * moon_init();
void moon_render(struct Moon * moon, Matrix4f pv, float frame,
        struct Vector4f * parent_pos, int simple, Matrix4f light_pv,
        float light_power);

struct Earth {
    struct Object * object;
    struct Object * orbit;
    struct Orbit * orbit_path;
    float rotation;
};


struct Earth * earth_init();
void earth_render(struct Earth* earth, Matrix4f pv, float frame,
        struct Moon * moon,
        int simple, Matrix4f light_pv,
        float light_power);

#endif
