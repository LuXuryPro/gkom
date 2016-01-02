#ifndef MATH3D_H
#define MATH3D_H
#include <math.h>
#define PI 3.14159265
#define RADIANS(deg) (deg) * PI / 180.0

struct Vector4f
{
    float x,y,z,w;
};

struct FrameOfReference
{
    struct Vector4f  up,
          forward;
    struct Vector4f position;

};

typedef float Matrix4f[16];

void vec4f_sum(struct Vector4f * a, struct Vector4f * b, struct Vector4f * result);
void vec4f_dot(struct Vector4f * a, struct Vector4f * b, float * result);
void vec4f_cross(struct Vector4f * a, struct Vector4f * b, struct Vector4f * result);
void vec4f_normalize(struct Vector4f * a);
void vec4f_flip(struct Vector4f * a);
void vec4f_scale(struct Vector4f * a, float factor);
float vec4f_lenght(struct Vector4f * a);
void frameofreference_to_mat4f(struct FrameOfReference *f, Matrix4f m);
void mat4f_vec_mul(Matrix4f a ,struct Vector4f *b);
void mat4f_mul(Matrix4f a, Matrix4f b, Matrix4f result);
void mat4f_init_perspective(Matrix4f matrix, float fov, float aspect,
        float zNear, float zFar);

void mat4f_init_identity(Matrix4f matrix);
void mat4f_translate(Matrix4f matrix, struct Vector4f * vector);
void mat4f_rot(Matrix4f matrix, struct Vector4f * axis, float angle);
void mat4f_scale(Matrix4f matrix, float scale);

#endif
