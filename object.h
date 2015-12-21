/*
 * Object contains mesh and material info
 *
 * */
#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"

struct Object {
    struct Mesh * mesh;
    GLuint program_id;
    GLuint texture_id;
};

#endif
