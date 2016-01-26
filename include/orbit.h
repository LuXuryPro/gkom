#ifndef ORBIT_H
#define ORBIT_H
#include "math3d.h"

struct Orbit {
    float eccentricity;
    float semimajor_axis;
    float inclination;
    float longitude_of_the_ascending_node;
    float argument_of_periapsis;
    float mean_anomaly;
    float peroid;
};

struct Orbit * init_orbit(
        float eccentricity,
        float semimajor_axis,
        float inclination,
        float longitude_of_the_ascending_node,
        float argument_of_periapsis,
        float peroid
        );

float mean_anomaly_to_eccentric_anamaly(float mean_anomaly,
        float eccentricity);
float orbit_get_true_anomaly(struct Orbit * orbit);
struct Vector4f orbit_get_position(struct Orbit * orbit);
void orbit_move(struct Orbit * orbir, float dt);


#endif
