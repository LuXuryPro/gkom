#include "orbit.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float eccentric_anomaly_to_true_anomaly(float eccentric_anomaly,
        float eccentricity)
{
    float sinf = sin(eccentric_anomaly)*sqrt(1 - eccentricity*eccentricity)/
        (1 - eccentricity* cos(eccentric_anomaly));
    float cosf = (cos(eccentric_anomaly) - eccentricity)/(1 - eccentricity*
            cos(eccentric_anomaly));
    return atan2(sinf, cosf); }

float mean_anomaly_to_eccentric_anamaly(float mean_anomaly,
        float eccentricity)
{
    float x;
    if ((mean_anomaly > -M_PI && mean_anomaly < 0) || (mean_anomaly > M_PI))
        x = mean_anomaly - eccentricity;
    else
        x = mean_anomaly + eccentricity;
    int i;
    for (i = 0; i < 20; i++) {
        float y = -mean_anomaly + x - eccentricity * sin(x);
        float yprime = 1 - eccentricity * cos(x);
        if (fabs(yprime) < 1e-10)
            break;
        float x1 = x - y / yprime;
        if (fabs((x1 - x)/x) < 1e-8)
            break;
        x = x1;
    }
    return x;
}


struct Orbit * init_orbit(float eccentricity, float semimajor_axis,
        float inclination, float longitude_of_the_ascending_node,
        float argument_of_periapsis, float peroid)
{
    struct Orbit * orbit = malloc(sizeof(struct Orbit));
    orbit->eccentricity = eccentricity;
    orbit->semimajor_axis = semimajor_axis;
    orbit->inclination = inclination;
    orbit->longitude_of_the_ascending_node = longitude_of_the_ascending_node;
    orbit->argument_of_periapsis = argument_of_periapsis;
    orbit->peroid = peroid;
    orbit->mean_anomaly = 0;
    return orbit;
}

float orbit_get_true_anomaly(struct Orbit * orbit)
{
    float eccentric_anomaly = mean_anomaly_to_eccentric_anamaly(
            orbit->mean_anomaly,
            orbit->eccentricity);
    return eccentric_anomaly_to_true_anomaly(eccentric_anomaly,
            orbit->eccentricity);
}

struct Vector4f orbit_get_position(struct Orbit * orbit)
{
    struct Vector4f vector;
    float angle = orbit_get_true_anomaly(orbit);
    float eps = orbit->semimajor_axis * ( 1 - orbit->eccentricity *
            orbit->eccentricity);
    float radius = eps/(1 + orbit->eccentricity * cos(angle));
    vector.x = radius * cos(angle);
    vector.y = 0;
    vector.z = radius * sin(angle);
    Matrix4f m;
    m[0] = cos(orbit->longitude_of_the_ascending_node) *
        cos(orbit->argument_of_periapsis) -
        sin(orbit->longitude_of_the_ascending_node) *
        cos(orbit->inclination) *
        sin(orbit->argument_of_periapsis);
    m[1] = sin(orbit->longitude_of_the_ascending_node) *
        cos(orbit->argument_of_periapsis) +
        cos(orbit->longitude_of_the_ascending_node) *
        cos(orbit->inclination) *
        sin(orbit->argument_of_periapsis);
    m[2] = sin(orbit->inclination) *
        sin(orbit->argument_of_periapsis);
    m[3] = 0;
    m[4] = - cos(orbit->longitude_of_the_ascending_node) *
        sin(orbit->argument_of_periapsis) -
        sin(orbit->longitude_of_the_ascending_node) *
        cos(orbit->inclination) *
        cos(orbit->argument_of_periapsis);
    m[5] = - sin(orbit->longitude_of_the_ascending_node) *
        sin(orbit->argument_of_periapsis) +
        cos(orbit->longitude_of_the_ascending_node) *
        cos(orbit->inclination) *
        cos(orbit->argument_of_periapsis);
    m[6] = sin(orbit->inclination) *
        cos(orbit->argument_of_periapsis);
    m[7] = 0;
    m[8] = sin(orbit->inclination) *
        sin(orbit->longitude_of_the_ascending_node);
    m[9] = - sin(orbit->inclination) *
        cos(orbit->longitude_of_the_ascending_node);
    m[10] = cos(orbit->inclination);
    m[11] = m[12] = m[13] = m[14] = 0;
    m[15] = 1;
    mat4f_vec_mul(m, &vector);
    return vector;
}


void orbit_move(struct Orbit * orbit, float dt)
{
    orbit->mean_anomaly += ((2 * M_PI)/orbit->peroid)*dt;
    if (orbit->mean_anomaly > 2 * M_PI)
        orbit->mean_anomaly = 0;
}
