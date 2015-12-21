#include "math3d.h"

void vec4f_cross(struct Vector4f * a, struct Vector4f * b, struct Vector4f * result) {
    result->x = (a->y * b->z) - (a->z * b->y);
    result->y = (a->z * b->x) - (a->x * b->z);
    result->z = (a->x * b->y) - (a->y * b->x);
}

void vec4f_dot(struct Vector4f * a, struct Vector4f * b, float * result) {
    *result = a->x * b->x + a->y * b->y + a->z * b->z;
}

float vec4f_lenght(struct Vector4f * a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}
void vec4f_normalize(struct Vector4f * a) {
    float lenght = vec4f_lenght(a);
    a->x /= lenght;
    a->y /= lenght;
    a->z /= lenght;
}

/*
 * For a given frame of reference capture matrix that translates object
 * to this frame of reference space.
 * */
void frameofreference_to_mat4f(struct FrameOfReference *f, Matrix4f m) {
    vec4f_normalize(&f->forward);
    vec4f_normalize(&f->up);
    struct Vector4f right;
    vec4f_cross(&f->forward, &f->up, &right);
    m[0] = right.x;
    m[4] = right.y;
    m[8] = right.z;
    m[3] = 0;
    m[1] = f->up.x;
    m[5] = f->up.y;
    m[9] = f->up.z;
    m[7] = 0;
    m[2] = f->forward.x;
    m[6] = f->forward.y;
    m[10] = f->forward.z;
    m[11] = 0;
    m[12] = -f->position.x * right.x - f->position.y * right.y - f->position.z * right.z;
    m[13] = -f->position.x * f->up.x -f->position.y * f->up.y - f->position.z * f->up.z;
    m[14] = -f->position.x * f->forward.x - f->position.y * f->forward.y -f->position.z * f->forward.z;
    m[15] = 1;
}

/*Combine two vectors together*/
void vec4f_sum(struct Vector4f * a, struct Vector4f * b, struct Vector4f * result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void vec4f_flip(struct Vector4f * a)
{
    a->x = -a->x;
    a->y = -a->y;
    a->z = -a->z;
}


void vec4f_scale(struct Vector4f * a, float factor)
{
    a->x = factor*a->x;
    a->y = factor*a->y;
    a->z = factor*a->z;
}

void mat4f_vec_mul(Matrix4f a ,struct Vector4f *b) {
    b->x = a[0] * b->x + a[4] * b->y + a[8] * b->z + a[12] * b->w;
    b->y = a[1] * b->x + a[5] * b->y + a[9] * b->z + a[13] * b->w;
    b->z = a[2] * b->x + a[6] * b->y + a[10] * b->z + a[14] * b->w;
    b->w = a[3] * b->x + a[7] * b->y + a[11] * b->z + a[15] * b->w;
}

void mat4f_rot(Matrix4f a, struct Vector4f * axis, float angle) {
    a[0] = cos(RADIANS(angle)) + axis->x * axis->x * (1 - cos(RADIANS(angle)));
    a[1] = axis->y * axis->x * (1 - cos(RADIANS(angle))) + axis->z * sin(RADIANS(angle));
    a[2] = axis->z * axis->x * (1 - cos(RADIANS(angle))) - axis->y * sin(RADIANS(angle));
    a[3] = 0;
    a[4] = axis->x * axis->y * (1 - cos(RADIANS(angle))) - axis->z * sin(RADIANS(angle));
    a[5] = cos(RADIANS(angle)) + axis->y * axis->y * (1 - cos(RADIANS(angle)));
    a[6] = axis->z * axis->y * (1 - cos(RADIANS(angle))) + axis->x * sin(RADIANS(angle));
    a[7] = 0;
    a[8] = axis->x * axis->z * (1 - cos(RADIANS(angle))) + axis->y * sin(RADIANS(angle));
    a[9] = axis->y * axis->z * (1 - cos(RADIANS(angle))) - axis->x * sin(RADIANS(angle));
    a[10] = cos(RADIANS(angle)) + axis->z * axis->z * (1 - cos(RADIANS(angle)));
    a[11] = 0;
    a[12] = 0;
    a[13] = 0;
    a[14] = 0;
    a[15] = 1;
}

/* result = a * b Order of a and b matters. */
void mat4f_mul(Matrix4f a, Matrix4f b, Matrix4f result) {
    int i;
    for (int i = 0; i < 16; i++) {
        result[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        /*For every row*/
        int j;
        for (j = 0; j < 16; j+=4) {
            /*For every column*/
            int k;
            for (k = 0; k < 4; k++) {
                result[j+i] += a[4*k + i] * b[k + j];
            }
        }
    }
}

